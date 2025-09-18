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
static unsigned long lastConnectionAttemp = 0;
static const unsigned long reconnectInterval = 1000 * 75; // 1 minuto e 15 segundos

// -----------------------------------------------------------------------------
// Funções
// -----------------------------------------------------------------------------

/**
 * @brief Tenta conectar à rede Wi-Fi.
 * 
 * - Inicia a conexão com o SSID e senha configurados.
 * - Aguarda até 5 segundos para a conexão ser estabelecida.
 * - Se conectado, obtém e registra o endereço IP.
 * 
 * @return true  Conexão estabelecida com sucesso.
 * @return false Falha ao conectar dentro do tempo limite.
 */
bool connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + 5000;
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
 * - Caso contrário, a cada 1 minuto e 15 segundos tenta reconectar à rede.
 * - Executa uma nova tentativa de conexão com timeout de 10 segundos.
 */
bool reconnectWifi() {
  // Se já está conectado, não faz nada
  if (WiFi.status() == WL_CONNECTED) return true;

  unsigned long now = millis();

  // Verifica se já passou o intervalo de reconexão
  if (now - lastConnectionAttemp >= reconnectInterval) {
    lastConnectionAttemp = now;

    log(LOG_ERROR,"WiFi desconectado");
    WiFi.disconnect();                       // garante que está desconectado
    log(LOG_INFO, "tentando reconectar com wifi...");
    return connectWiFi();
  }
  return false;
}
