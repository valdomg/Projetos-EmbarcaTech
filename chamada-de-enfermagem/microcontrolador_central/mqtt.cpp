#include <WiFiClientSecure.h>
#include "mqtt.h"
#include "config.h"

WiFiClientSecure espClient;
PubSubClient client(espClient);


static unsigned long lastAttempConnectMQTT = 0;

static const unsigned long reconnectIntervalMQTT = 3000;

/**
 * Configura a conexão com o servidor MQTT
 */
void setupMQTT() {
  espClient.setInsecure();  // Não verifica certificado
  client.setServer(MQTT_SERVER, 8883);
  client.setCallback(callback);
}

/**
 * Verifica a conexão com MQTT, e reconecta se necessario.
 */
void checkMQTTConnected() {

  client.loop();
  if (client.connected()) return;

  unsigned long now = millis();

  if (now - lastAttempConnectMQTT >= reconnectIntervalMQTT){

    lastAttempConnectMQTT = now; 
    Serial.print("Tentando conectar ao MQTT... ");
    if (client.connect(MQTT_DEVICE_ID, MQTT_USER, MQTT_PASS)) {
      Serial.println("Conectado!");
      client.subscribe(MQTT_SUBSCRIPTION_TOPIC);
    } else {
      Serial.print("Erro, rc=");
      Serial.println(client.state());
    }
  }
}

// Função callback: chamada quando uma mensagem é recebida
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  Serial.print("Mensagem: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/**
 * Publica um valor float formatado para duas casas decimais em um tópico MQTT.
 * 
 * @param topic - Tópico MQTT para publicação.
 * @param value - Valor numérico a ser enviado.
 */
void publicReponseDivice(const char* id, float value) {
  char payload[10];
  snprintf(payload, sizeof(payload), "%.2f", value);

  char topic[50];
  snprintf(topic, sizeof(topic), "%s/%s", MQTT_PUBLICATION_TOPIC, id);

  client.publish(topic, payload);
}
