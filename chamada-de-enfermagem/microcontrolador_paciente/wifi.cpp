#include <ESP8266WiFi.h>
#include "wifi.h"


const char* WIFI_SSID = "XXXXXXX";
const char* WIFI_PASSWORD = "XXXXXXX";



unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 10000;

bool connectWiFi() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  unsigned long timeout = millis() + 10000;
  while (
    WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado ao Wi-Fi");
    Serial.println(WiFi.localIP());
    return true;
  } else{
    Serial.println("Falha na conexão Wi-Fi");
    return false;
  }
}

void checkConnection(){
 unsigned long currentMillis = millis();
 
 if (WiFi.status() != WL_CONNECTED && (currentMillis - lastReconnectAttempt >= reconnectInterval)) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    connectWiFi();

    lastReconnectAttempt = currentMillis;
 }
}