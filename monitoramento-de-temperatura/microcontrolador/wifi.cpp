#include <ESP8266WiFi.h>
#include "wifi.h"
#include "log.h"

const char* WIFI_SSID = "xxxxxxxxxx";
const char* WIFI_PASSWORD = "xxxxxxxx";

char ipBuffer[16];

/**
 * Conecta com WiFi e retorna valor true para conexão bem sucedida.
 */
bool connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + 10000;
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);

  }

  if (WiFi.status() == WL_CONNECTED) {

    log(LOG_INFO, "Conectado a rede wifi %s", WIFI_SSID);
    IPAddress ip = WiFi.localIP();

    // Converte diretamente para const char*
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u",
             ip[0], ip[1], ip[2], ip[3]);
    log(LOG_INFO, "IP: %s", ipBuffer);

    return true;
  }

  return false;
}
