#include "mqtt.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "log.h"

const char* MQTT_SERVER = "xxxxxxxxxxx";
const char* MQTT_USER = "xxxxxxxxxxxx";
const char* MQTT_PASS = "xxxxxxxxxxxx";
const char* MQTT_TOPIC_PUBLICATION_DATA = "xxxxxxxxxxxx/xxxxxxxxxxxx";
const char* MQTT_TOPIC_PUBLICATION_ALERT = "xxxxxxxxxxxx/xxxxxxxxxxxx";
const char* MQTT_DEVICE_ID = "xxxxxxxxxxxx";

WiFiClientSecure espClient;
PubSubClient client(espClient);

/**
 * Configura a conexão com o servidor MQTT
 */
void setupMQTT() {
  espClient.setInsecure();  // Não verifica certificado
  client.setServer(MQTT_SERVER, 8883);
  log(LOG_INFO, "MQTT inicializado");
}

/**
 * Verifica a conexão com MQTT, e reconecta se necessario.
 */
void checkMQTTConnected() {
  if (!client.connected()) {
    while (!client.connected()) {
      if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PASS)) {
        log(LOG_INFO, "Conectado ao Broker MQTT");
      } else {
        log(LOG_ERROR, "Erro ao conectar com Broker MQTT, numero do erro: %d", client.state());
        delay(5000);
      }
    }
  }
}


/** 
 * Realiza a leitura do sensor e publica os dados via MQTT.
 *
 * @param temperature - valor da temperatura.
 * @param humidity -  valor da umidade.
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

void publishAlert(const char* alert) {

  StaticJsonDocument<96> doc;
  doc["Microcontrollerid"] = MQTT_DEVICE_ID;
  doc["alert"] = alert;

  char buffer[96];

  serializeJson(doc, buffer);
  client.publish(MQTT_TOPIC_PUBLICATION_ALERT, buffer);
}
