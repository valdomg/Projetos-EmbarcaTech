#include "wifi.h"
#include "mqtt.h"

unsigned long lastPublish = 0;
const long interval = 60000;

void setup() {

  Serial.begin(115200);
  connectWiFi();
  connectMQTT();

}

void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
  checkConnection();

  //Publica a cada 60 segundos(para testes)
  if (millis() - lastPublish > interval) {
    publishData();
    lastPublish = millis();
  }
}