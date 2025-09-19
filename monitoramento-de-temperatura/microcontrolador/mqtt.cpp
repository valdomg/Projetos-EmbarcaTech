#include "mqtt.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "log.h"

/**
 * @file mqtt.cpp
 * @brief Implementação das funções relacionadas à comunicação MQTT.
 *
 * Este módulo gerencia a configuração, conexão e publicação
 * de dados em tópicos MQTT utilizando comunicação segura (TLS/SSL).
 */

// -----------------------------------------------------------------------------
// Constantes de configuração do MQTT
// -----------------------------------------------------------------------------

/// Endereço do broker MQTT.
const char* MQTT_SERVER = "xxxxxxxxxxx";

/// Usuário para autenticação no broker MQTT.
const char* MQTT_USER = "xxxxxxxxxxxx";

/// Senha para autenticação no broker MQTT.
const char* MQTT_PASS = "xxxxxxxxxxxx";

/// Tópico para publicação de dados de sensores (temperatura/umidade).
const char* MQTT_TOPIC_PUBLICATION_DATA = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Tópico para publicação de alertas.
const char* MQTT_TOPIC_PUBLICATION_ALERT = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Identificação única do dispositivo no broker MQTT.
const char* MQTT_DEVICE_ID = "xxxxxxxxxxxx";

// -----------------------------------------------------------------------------
// Objetos MQTT
// -----------------------------------------------------------------------------

/// Cliente seguro para conexão TLS/SSL.
WiFiClientSecure espClient;

/// Cliente MQTT baseado na biblioteca PubSubClient.
PubSubClient client(espClient);

// -----------------------------------------------------------------------------
// Variáveis de controle
// -----------------------------------------------------------------------------
static unsigned long lastAttempConnectMQTT = 0;         // Guarda o tempo da última tentativa de conexão com o broker.
static const unsigned long reconnectIntervalMQTT = 3000; // Intervalo (ms) entre tentativas de reconexão ao broker.

// -----------------------------------------------------------------------------
// Funções de inicialização e conexão
// -----------------------------------------------------------------------------

/**
 * @brief Configura a conexão com o servidor MQTT.
 *
 * Define as credenciais e parâmetros necessários para
 * estabelecer a comunicação com o broker.
 * Neste caso, o certificado não é validado (uso de `setInsecure()`).
 */
void setupMQTT() {
  espClient.setInsecure();  // Não verifica certificado
  client.setServer(MQTT_SERVER, 8883);
  log(LOG_DEBUG, "MQTT inicializado");
}

/**
 * @brief Verifica a conexão com o broker MQTT e reconecta se necessário.
 * 
 * A cada chamada, mantém a conexão ativa (client.loop()).
 * Se a conexão caiu, tenta reconectar dentro do intervalo definido em reconnectIntervalMQTT.
 */
void checkMQTTConnected() {
  client.loop();                         // Mantém a comunicação ativa e processa mensagens recebidas.
  if (client.connected()) return;        // Se já está conectado, sai da função.

  unsigned long now = millis();          // Captura o tempo atual (ms desde o boot).

  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT){
    lastAttempConnectMQTT = now;         // Atualiza o tempo da última tentativa

    log(LOG_DEBUG,"Tentando conectar ao MQTT...");
    // Tenta conectar ao broker usando credenciais do config.h
    if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PASS)) {
      log(LOG_DEBUG,"Conectado ou Broker");
    } else {
      log(LOG_ERROR,"Erro ao conectar com Broker rc= %d",client.state()); // Mostra o código de erro da conexão
    }
  }
}

// -----------------------------------------------------------------------------
// Funções de publicação
// -----------------------------------------------------------------------------

/**
 * @brief Publica os dados de sensores (temperatura e umidade) no broker MQTT.
 *
 * O payload é estruturado em formato JSON contendo:
 * - Identificação do microcontrolador
 * - Valor da temperatura
 * - Valor da umidade
 *
 * @param temperature Valor da temperatura a ser enviado.
 * @param humidity    Valor da umidade a ser enviado.
 */
void publishSensorData(float temperature, float humidity) {
  StaticJsonDocument<128> doc;
  doc["Microcontrollerid"] = MQTT_DEVICE_ID;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char buffer[128];
  serializeJson(doc, buffer);
  client.publish(MQTT_TOPIC_PUBLICATION_DATA, buffer);
}

/**
 * @brief Publica uma mensagem de alerta no broker MQTT.
 *
 * O payload é estruturado em formato JSON contendo:
 * - Identificação do microcontrolador
 * - Mensagem de alerta
 *
 * @param alert String com a mensagem de alerta.
 */
void publishAlert(const char* alert) {
  StaticJsonDocument<96> doc;
  doc["Microcontrollerid"] = MQTT_DEVICE_ID;
  doc["alert"] = alert;

  char buffer[96];
  serializeJson(doc, buffer);
  client.publish(MQTT_TOPIC_PUBLICATION_ALERT, buffer);
}