/**
 * @file mqtt.cpp
 * @brief Implementação do módulo de comunicação MQTT.
 *
 * @details
 * Responsável por:
 * - Configurar cliente MQTT;
 * - Gerenciar reconexão automática ao broker;
 * - Publicar mensagens de solicitação;
 * - Receber comandos via subscribe;
 * - Processar mensagens JSON;
 * - Controlar LED e estado do botão com base nas mensagens recebidas.
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "mqtt.h"
#include "led.h"

/* ============================
   Variáveis de Controle
   ============================ */

/**
 * @brief Timestamp da última tentativa de conexão MQTT.
 */
static unsigned long lastAttempConnectMQTT = 0;

/**
 * @brief Intervalo entre tentativas de reconexão (ms).
 */
static const unsigned long reconnectIntervalMQTT = 3000;

/**
 * @brief Controle externo do estado do botão.
 * 
 * true  → botão bloqueado  
 * false → botão liberado
 */
extern bool buttonEnable;


/* ============================
   Configuração do Broker
   ============================ */

/**
 * @brief Endereço do broker MQTT.
 */
const char* MQTT_BROKER = "";

/**
 * @brief Porta do broker MQTT.
 */
const int MQTT_PORT = 0;

/**
 * @brief Credenciais e identificação do cliente.
 */
const char* MQTT_USER = "";
const char* MQTT_PASS = "";
const char* ID_CLIENT = "Idclient";


/* ============================
   Tópicos MQTT
   ============================ */

const char* TOPIC_PUBLISH = "dispositivos/posto_enfermaria/Idclient";  //ENVIA EMERGENCIA/OCIOSO
const char* TOPIC_CONFIRMACAO = "dispositivos/confirmacao/Idclient";
const char* TOPIC_ATENDIMENTO = "dispositivos/atendimento/Idclient";



/* ============================
   Objetos MQTT
   ============================ */

WiFiClient espClient;
PubSubClient client(espClient);


/**
 * @brief Inicializa configuração do cliente MQTT.
 */
void setupMQTT() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
}


/**
 * @brief Realiza conexão com o broker MQTT.
 *
 * @details
 * Implementa controle de reconexão com intervalo mínimo
 * definido por reconnectIntervalMQTT.
 */
void connectMQTT() {

  unsigned long now = millis();

  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;

    Serial.println("Tentando conectar ao MQTT");

    if (client.connect(ID_CLIENT, MQTT_USER, MQTT_PASS)) {

      Serial.println("Exito na conexão");

      client.subscribe(TOPIC_CONFIRMACAO);
      client.subscribe(TOPIC_ATENDIMENTO);


    } else {

      Serial.print("Falha ao conectar: ");
      Serial.println(client.state());
      Serial.println();
    }
  }
}


/**
 * @brief Cria payload JSON da solicitação de atendimento.
 *
 * @return Ponteiro para string JSON serializada.
 */
const char* createJsonPayload(const char* estado, const char* mensagem, const char* comando) {

  StaticJsonDocument<200> doc;

  doc["id"] = ID_CLIENT;
  doc["estado"] = estado;
  doc["mensagem"] = mensagem;
  doc["comando"] = comando;

  doc["room_number"] = 1;
  doc["local"] = "enfermaria";

  static char payload[200];
  serializeJson(doc, payload);

  return payload;
}


/**
 * @brief Callback executado ao receber mensagem MQTT.
 *
 * @param topic  Tópico onde a mensagem foi recebida.
 * @param payload Conteúdo da mensagem.
 * @param length Tamanho do payload.
 *
 * @details
 * Fluxo:
 * - Se mensagem for do TOPIC_SUBSCRIBE:
 *      - Processa comando "desligar"
 *      - Desliga LED
 *      - Libera botão
 *      - Envia confirmação
 *
 * - Se mensagem for do TOPIC_SUBSCRIBE_CONFIRM:
 *      - Se status = "ok"
 *      - Liga LED
 *      - Bloqueia botão
 */
void callback(char* topic, byte* payload, unsigned int length) {

  StaticJsonDocument<200> doc;

  if (length == 0) {
    Serial.println("Payload vazio recebido");
    return;
  }

  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Erro ao parsear JSON: ");
    Serial.println(error.c_str());
    return;
  }

  if (strcmp(topic, TOPIC_ATENDIMENTO) == 0)
   {

    if (doc.containsKey("atendimento") && !doc["atendimento"].isNull()) {

      const char* comando = doc["atendimento"];
      Serial.print("Comando: ");
      Serial.println(comando);
      
      if (strcmp(comando, "atendido") == 0){


        if (client.publish(TOPIC_PUBLISH, createJsonPayload("ocioso", "atendido", "desligar"), true)) {
          
          desligarLed();
          buttonEnable = false;

          Serial.println("Enviado estado ocioso!");
        }
      

      } else if (strcmp(comando, "finalizado") == 0) {

        desligarLed();
        buttonEnable = false;

        Serial.println("Solicitação finalizada. Botão liberado!");

        if (client.publish(TOPIC_PUBLISH, "", true)) {
          //pode ser chamado logo apos receber o comando destravar/atendido
          Serial.println("Enviado payload vazio.");
        }
      }

    } else {
      Serial.println("Comando ausente ou nulo");
    }

  } else if (strcmp(topic, TOPIC_CONFIRMACAO) == 0) {

    if (doc.containsKey("status") && !doc["status"].isNull()) {

      const char* status = doc["status"];
      
      Serial.print("Status: ");
      Serial.println(status);

      if (strcmp(status, "ok") == 0){

        Serial.println("Solicitação recebida pelo enfermeiro");

        ligarLed();
        buttonEnable = true;
      }

    } else {
      Serial.println("Comando ausente ou nulo");
    }
  }
}