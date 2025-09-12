#include "mqtt.h"
#include <WiFiClientSecure.h>
#include "log.h"

const char* MQTT_SERVER = "xxxxxxxxxxx";
const char* MQTT_USER = "xxxxxxxxxxxx";
const char* MQTT_PASS = "xxxxxxxxxxxx";

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
      if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASS)) {
        log(LOG_INFO, "Conectado ao Broker MQTT");
      } else {
        log(LOG_ERROR, "Erro ao conectar com Broker MQTT, numero do erro: %d", client.state());
        delay(5000);
      }
    }
  }
}

/**
 * Publica um valor float formatado para duas casas decimais em um tópico MQTT.
 * 
 * @param topic - Tópico MQTT para publicação.
 * @param value - Valor numérico a ser enviado.
 */
void publishFloat(const char* topic, float value) {
  char payload[10];
  snprintf(payload, sizeof(payload), "%.2f", value);
  client.publish(topic, payload);
}

/** 
 * Realiza a leitura do sensor e publica os dados via MQTT.
 *
 * @param temperature - valor da temperatura.
 * @param humidity -  valor da umidade.
 */
void publishSensorData(float temperature, float humidity) {
  publishFloat("laboratorio/temperatura", temperature);
  publishFloat("laboratorio/humidade", humidity);
}
