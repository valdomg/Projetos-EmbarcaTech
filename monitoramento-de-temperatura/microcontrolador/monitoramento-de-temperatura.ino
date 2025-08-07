#include "sensor.h"
#include "wifi.h"
#include "mqtt.h"

unsigned long previousMQTTTime = 0;
const unsigned long MQTTInterval = 180000; // 3 segundo

unsigned long previousDisplay = 0;
unsigned long displayInterval = 1000;  // 1 segundo

void setup() {
  Serial.begin(115200);
  initializeSensor();
  if (!connectWiFi()) {
    Serial.println("WiFi não conectado.");
  }
  setupMQTT();
}

void loop() {
  client.loop();
  checkMQTTConnected();

  unsigned long now = millis();
  if (now - previousMQTTTime >= MQTTInterval) {
    previousMQTTTime = now;
    publishSensorData();
  }
  if (now - previousDisplay >= displayInterval) {
    previousDisplay = now;
    // Logica do display
  }
}
