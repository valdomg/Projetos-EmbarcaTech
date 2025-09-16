#include <ESP8266WiFi.h>
#include "wifi.h"
#include "log.h"

// -----------------------------------------------------------------------------
// Credenciais da rede Wi-Fi
// -----------------------------------------------------------------------------
const char* WIFI_SSID = "xxxxxxxxxx";      ///< Nome da rede Wi-Fi (SSID)
const char* WIFI_PASSWORD = "xxxxxxxx";    ///< Senha da rede Wi-Fi

// -----------------------------------------------------------------------------
// Buffer e controle de tempo
// -----------------------------------------------------------------------------
char ipBuffer[16];                         ///< Armazena o IP em formato string
unsigned long previousMillis = 0;          ///< Marca do último tempo de tentativa
const long interval = 10000;               ///< Intervalo entre tentativas de reconexão (10s)

// -----------------------------------------------------------------------------
// Funções
// -----------------------------------------------------------------------------

/**
 * @brief Tenta conectar à rede Wi-Fi.
 * 
 * - Inicia a conexão com o SSID e senha configurados.
 * - Aguarda até 10 segundos para a conexão ser estabelecida.
 * - Se conectado, obtém e registra o endereço IP.
 * 
 * @return true  Conexão estabelecida com sucesso.
 * @return false Falha ao conectar dentro do tempo limite.
 */
bool connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + 5000;  // Tempo limite de 5 segundos
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);  // Pequena espera antes de verificar novamente
  }

  if (WiFi.status() == WL_CONNECTED) {
    log(LOG_INFO, "Conectado a rede wifi %s", WIFI_SSID);

    IPAddress ip = WiFi.localIP();
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u",
             ip[0], ip[1], ip[2], ip[3]);

    log(LOG_INFO, "IP: %s", ipBuffer); 
    return true;
  }

  return false;
}

/**
 * @brief Garante que o dispositivo esteja conectado ao Wi-Fi.
 * 
 * - Se já estiver conectado, não faz nada.
 * - Caso contrário, a cada 10 segundos tenta reconectar à rede.
 * - Executa uma nova tentativa de conexão com timeout de 10 segundos.
 */
void reconnectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long timeout = millis() + 5000;
    while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
      delay(500);
    }
  }
}