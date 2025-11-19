#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "mqtt.h"
#include "led.h"


extern bool buttonBlocked;

// MQTT Broker
const char* MQTT_BROKER = "xxx";
const int MQTT_PORT = 0;
const char* MQTT_USER = "enfermaria1";
const char* MQTT_PASS = "123";
const char* ID_CLIENT = "enfermaria1";
const char* TOPIC_PUBLISH = "dispositivos/posto_enfermaria";
const char* TOPIC_SUBSCRIBE = "dispositivos/enfermaria/enfermaria1";
const char* TOPIC_SUBSCRIBE_CONFIRM = "dispositivo/confirmacao/enfermaria1";

WiFiClient espClient;
PubSubClient client(espClient);




void connectMQTT() {

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);

  if (client.connect(ID_CLIENT, MQTT_USER, MQTT_PASS)) {
    Serial.println("Exito na conexão");
    Serial.printf("Cliente %s conectado ao Broker\n", ID_CLIENT);
    client.subscribe(TOPIC_SUBSCRIBE);
    client.subscribe(TOPIC_SUBSCRIBE_CONFIRM);

  } else {
    Serial.print("Falha ao conectar: ");
    Serial.println(client.state());
    Serial.println();
  }
}



//funcao publicar dados no topico(para testes)
void publishData() {
  // Criando o objeto JSON
  StaticJsonDocument<200> doc;
  doc["id"] = ID_CLIENT;
  doc["estado"] = "emergencia";
  doc["mensagem"] = "solicitar atendimento";
  doc["room_number"] = 1;
  doc["local"] = "enfermaria";
  doc["comando"] = "ligar";


  char payload[200];
  serializeJson(doc, payload);

  // Publicando o JSON no tópico
  client.publish(TOPIC_PUBLISH, payload);
}



void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Erro ao parsear JSON: ");
    Serial.println(error.c_str());
    return;
  }


  if (String(topic) == TOPIC_SUBSCRIBE) {

    if (doc.containsKey("comando") && !doc["comando"].isNull()) {
      const char* comando = doc["comando"];
      Serial.println("Comando: " + String(comando));

      if (String(comando) == "desligar") {
        desligarLed();
        buttonBlocked = false;
        Serial.println("Botão liberado");
      }
    } else {
      Serial.println("Comando ausente ou nulo");
    }

  }

  else if (String(topic) == TOPIC_SUBSCRIBE_CONFIRM) {

    if (doc.containsKey("status") && !doc["status"].isNull()) {
      const char* status = doc["status"];
      Serial.println("Status: " + String(status));

      if (String(status) == "ok") {
        ligarLed();
        buttonBlocked = true;
        Serial.println("Botão bloqueado");
      }
    } else {
      Serial.println("Comando ausente ou nulo");
    }
  }
}
