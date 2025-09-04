#include "wifi_utils.h"

void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }
}

void loop() {
  
  checkAndReconnectWifi();
  delay(1000);
}
