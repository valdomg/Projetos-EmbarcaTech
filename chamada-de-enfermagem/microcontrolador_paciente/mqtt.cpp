#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "mqtt.h"



// MQTT Broker
const char* MQTT_BROKER = "XXXXXX";
const int MQTT_PORT = XXXXXX;
// const char* MQTT_USER = "";
// const char* MQTT_PASS = "";
const char* ID_CLIENT = "XXXXXX";
const char* TOPIC = "dispositivos/posto_enfermagem/ID_CLIENT";



WiFiClient espClient;
PubSubClient client(espClient);




void connectMQTT() {

  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);

  if (client.connect(ID_CLIENT)) {
    Serial.println("Exito na conexão");
    Serial.printf("Cliente %s conectado ao Broker\n", ID_CLIENT);
    client.subscribe(TOPIC);

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
  client.publish(TOPIC, payload);
}



void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (!error) {

    if (doc.containsKey("comando") && !doc["comando"].isNull()) {
      const char* comando = doc["comando"];
      Serial.println("Comando: " + String(comando));

      if (String(comando) == "desligar") {
        Serial.println("Desliado");
      }
    } else {
      Serial.println("Comando ausente ou nulo");
    }
  } else {
    Serial.print("Erro ao parsear JSON: ");
    Serial.println(error.c_str());
  }
}

