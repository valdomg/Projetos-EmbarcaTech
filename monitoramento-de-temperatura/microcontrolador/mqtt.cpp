#include "config.h"
#include "mqtt.h"
#include "log.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "storage.h"
#include "config_storage.h"

/**
 * @file mqtt.cpp
 * @brief Implementação das funções relacionadas à comunicação MQTT.
 *
 * Este módulo gerencia a configuração, conexão e publicação
 * de dados em tópicos MQTT utilizando comunicação segura (TLS/SSL).
 */


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
static unsigned long lastAttempConnectMQTT = 0;           // Guarda o tempo da última tentativa de conexão com o broker.
static const unsigned long reconnectIntervalMQTT = 3000;  // Intervalo (ms) entre tentativas de reconexão ao broker.


static unsigned long lastConnectionDataSent = 0;       // Guarda o tempo da último envio de mensagem do storage.
static const unsigned long resendIntervalMQTT = 3000;  // Intervalo (ms) entre o envio de cada mensagem ao mqtt do storage.

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
  client.setServer(cfg.mqttServer.c_str(), 8883);
  log(LOG_DEBUG, "MQTT inicializado");
}

/**
 * @brief Verifica a conexão com o broker MQTT e reconecta se necessário.
 * 
 * A cada chamada, mantém a conexão ativa (client.loop()).
 * Se a conexão caiu, tenta reconectar dentro do intervalo definido.
 */
bool checkMQTTConnected() {
  client.loop();                        // Mantém a comunicação ativa e processa mensagens recebidas.
  if (client.connected()) return true;  // Se já está conectado, sai da função.

  unsigned long now = millis();  // Captura o tempo atual (ms desde o boot).

  // Só tenta reconectar se já passou o intervalo configurado
  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT) {
    lastAttempConnectMQTT = now;  // Atualiza o tempo da última tentativa

    log(LOG_DEBUG, "Tentando conectar ao MQTT...");
    // Tenta conectar ao broker usando credenciais do config.h
    if (client.connect(cfg.mqttDeviceId.c_str(), cfg.mqttUser.c_str(), cfg.mqttPass.c_str())) {
      log(LOG_DEBUG, "Conectado ou Broker");
      
      return true;
    } else {
      log(LOG_ERROR, "Erro ao conectar com Broker rc= %d", client.state());  // Mostra o código de erro da conexão
      return false;
    }
  }
  return false;
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
bool publishSensorData(float temperature, float humidity) {
  StaticJsonDocument<128> doc;
  doc["Microcontrollerid"] = cfg.mqttDeviceId.c_str();
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;


  char buffer[128];
  serializeJson(doc, buffer);

  if (!client.publish(cfg.mqttTopicData.c_str(), buffer)) {
    log(LOG_WARN, "falha ou enviar dados ao broker");
    return false;
  }
  return true;
}


void resendMqttData() {

  if (!hasDataStorage()) return;

  unsigned long now = millis();

  if (now - lastConnectionDataSent >= resendIntervalMQTT) {
    lastConnectionDataSent = now;  // Atualiza o tempo da última tentativa

    ObjectStorage obj = getObjectStorage();
    if (obj.valid) {
      if (publishSensorData(obj.temperature, obj.humidity)) {
        deleteOneMessage();
      }
    }
  }
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
  doc["Microcontrollerid"] = cfg.mqttDeviceId.c_str();
  doc["alert"] = alert;

  char buffer[96];
  serializeJson(doc, buffer);
  client.publish(cfg.mqttTopicAlert.c_str(), buffer);
}
