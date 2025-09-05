#include "wifi_utils.h"
#include "mqtt.h"

const int BOTAO = 33;

void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }
  setupMQTT();
}

void loop() {
  
  checkAndReconnectWifi();
  client.loop();
  checkMQTTConnected();
  
  delay(1000);
}
