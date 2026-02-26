#include <ESP8266WiFi.h>
#include "wifi.h"
#include "log.h"
#include "config.h"
#include "config_storage.h"
#include "display_LCD-1602_I2C.h"

// -----------------------------------------------------------------------------
// Buffer e controle de tempo
// -----------------------------------------------------------------------------

char ipBuffer[16];  ///< Armazena o endereço IP do dispositivo em formato de string (ex: "192.168.1.10")

static unsigned long lastConnectionAttemp = 0; ///< Marca o tempo da última tentativa de conexão
static const unsigned long reconnectInterval = 1000 * 75; ///< Intervalo de reconexão (1 minuto e 15 segundos)

// -----------------------------------------------------------------------------
// Funções
// -----------------------------------------------------------------------------

const char* IPparserToConstChar(IPAddress ip) {
  static char buffer[16];
  snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

  return buffer;
}

/**
 * @brief Conecta o dispositivo à rede Wi-Fi configurada.
 * 
 * Esta função utiliza o SSID e senha definidos em `cfg.wifiSSID` e `cfg.wifiPass`.
 * São obtidos a partir da estrutura global `cfg`, que contém as variáveis persistidas.
 * Ela tenta estabelecer uma conexão com a rede Wi-Fi e aguarda até 5 segundos
 * para o processo ser concluído.
 * 
 * - Se a conexão for bem-sucedida, o endereço IP é obtido e registrado no log.
 * - Caso contrário, retorna `false` indicando falha na conexão.
 * 
 * @return true  Se a conexão foi estabelecida com sucesso.
 * @return false Se não foi possível conectar dentro do tempo limite.
 */
bool connectWiFi() {
  WiFi.begin(cfg.wifiSSID.c_str(), cfg.wifiPass.c_str());

  unsigned long timeout = millis() + 5000;
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);  // Aguarda meio segundo antes de verificar novamente
  }

  if (WiFi.status() == WL_CONNECTED) {
    log(LOG_INFO, "Conectado à rede Wi-Fi %s", cfg.wifiSSID.c_str());

    IPAddress ip = WiFi.localIP();
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u",
             ip[0], ip[1], ip[2], ip[3]);

    log(LOG_INFO, "Endereço IP: %s", ipBuffer);
    return true;
  }

  return false;
}

/**
 * @brief Mantém a conexão Wi-Fi ativa, tentando reconectar quando necessário.
 * 
 * Esta função deve ser chamada periodicamente (ex: no loop principal).
 * 
 * - Se o dispositivo estiver conectado, nada é feito.
 * - Caso contrário, a cada 1 minuto e 15 segundos, uma nova tentativa de conexão é feita.
 * - Se uma tentativa for feita, a função chama internamente `connectWiFi()`.
 * 
 * @return true  Se o dispositivo está ou foi reconectado com sucesso.
 * @return false Se ainda não está conectado e o tempo de reconexão não chegou.
 */
bool reconnectWifi() {
  // Se já está conectado, retorna imediatamente
  if (WiFi.status() == WL_CONNECTED) return true;

  unsigned long now = millis();

  // Verifica se já passou o intervalo mínimo para tentar reconectar
  if (now - lastConnectionAttemp >= reconnectInterval) {
    lastConnectionAttemp = now;

    log(LOG_ERROR, "Wi-Fi desconectado");
    WiFi.disconnect();  // Garante que qualquer conexão antiga seja encerrada
    log(LOG_INFO, "Tentando reconectar ao Wi-Fi...");
    return connectWiFi();
  }

  return false;
}

/**
 * @brief Cria um ponto de acesso (Access Point) para configuração local.
 * 
 * Caso o dispositivo não esteja em modo AP, esta função:
 * - Desconecta o Wi-Fi atual.
 * - Configura o modo `WIFI_AP`.
 * - Inicializa um Access Point com o SSID e senha definidos nas constantes
 *   `SSID_ACCESS_POINT` e `PASSWORD_ACCESS_POINT`.
 * 
 * Também imprime o endereço IP do AP via `Serial`.
 */
void createAccessPoint() {
  WiFiMode_t mode = WiFi.getMode();

  if (mode != WIFI_AP) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID_ACCESS_POINT, PASSWORD_ACCESS_POINT);
    IPAddress ip = WiFi.softAPIP();
    
    showIPAddress(IPparserToConstChar(ip));
    log(LOG_INFO, "Endereço IP: %s", ipBuffer);
  }
}
