#include "wifi_utils.h"
#include "display_LCD-2004_I2C.h"
#include "DoublyLinkedList_NursingCall.h"


List_NursingCall listCalls;


void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }

  // inicializa o display
  lcd2004_init();

}

void loop() {
  
  checkAndReconnectWifi();
  delay(1000);

}
