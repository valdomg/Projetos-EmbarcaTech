#include "wifi_utils.h"
#include "display_LCD-2004_I2C.h"


void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }

  lcd2004_init();
}

void loop() {
  
  checkAndReconnectWifi();
  delay(1000);
}
