#include <ESP8266WiFi.h>  // Biblioteca oficial ESP32 para conexão Wi-Fi
#include "wifi_utils.h"   // Declarações das funções públicas do módulo Wi-Fi
#include "config.h"       // Constantes globais do projeto (SSID e senha)
#include "log.h"
#include "config_storage.h"

#include "display_LCD-2004_I2C.h" // Para mostrar o IP da rede


// ------------------------------------------------------------
// Variáveis internas do módulo
// ------------------------------------------------------------

char ipBuffer[16];  ///< Armazena o endereço IP do dispositivo em formato de string (ex: "192.168.1.10")

// ------------------------------------------------------------
// Funções uteis para o módulo
// ------------------------------------------------------------
const char* IPparserToConstChar(IPAddress ip) {
  static char buffer[16];
  snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);

  return buffer;
}



/**
 * @brief Armazena o timestamp (millis) da última tentativa de reconexão.
 * Usado para limitar a frequência de tentativas automáticas.
 */
static unsigned long lastConnectionAttemp = 0;

/**
 * @brief Intervalo mínimo entre tentativas de reconexão (ms)
 * Evita reconectar muito rápido quando o Wi-Fi está instável.
 */
static const unsigned long reconnectInterval = 2000;  // 10 segundos

// ------------------------------------------------------------
// Implementação das funções públicas do módulo
// ------------------------------------------------------------

/**
 * @brief Conecta o ESP32 à rede Wi-Fi definida em config.h.
 * 
 * Aguarda até 10 segundos pela conexão, imprimindo progresso no Serial.
 * 
 * @return true se a conexão foi bem sucedida, false caso contrário.
 */
bool connectToWiFi() {
  WiFi.begin(cfg.wifiSSID.c_str(), cfg.wifiPass.c_str());  // inicia conexão
  log(LOG_INFO, "Conctandoo ao wifi");

  // Define um timeout de 10 segundos
  unsigned long timeout = millis() + 5000;

  // Aguarda até a conexão ou até o timeout
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);  // aguarda meio segundo entre tentativas
    Serial.print(".");     // imprime ponto para indicar progresso
  }

  Serial.println();

  // Verifica se a conexão foi bem sucedida
  if (WiFi.status() == WL_CONNECTED) {
    log(LOG_INFO, "Conectado ao Wi-Fi: %s", WIFI_SSID);
    const char* network_ipAddress = IPparserToConstChar(WiFi.localIP());
    // log(LOG_INFO, "IP: %s", IPparserToConstChar(WiFi.localIP()));  // mostra o IP atribuído
    log(LOG_INFO, "IP: %s", network_ipAddress);  // mostra o IP atribuído

    // Mostrar no display o IP da rede
    showIPAddress(network_ipAddress);
    return true;
  }

  log(LOG_ERROR, "Falha na conexão Wi-Fi");
  return false;
}

/**
 * @brief Verifica a conexão Wi-Fi e tenta reconectar automaticamente.
 * 
 * Essa função deve ser chamada periodicamente no loop() do programa.
 * Se o Wi-Fi estiver desconectado, ela espera o intervalo definido
 * em reconnectInterval antes de tentar reconectar.
 */
bool checkAndReconnectWifi() {
  // Se já está conectado, não faz nada
  if (WiFi.status() == WL_CONNECTED) return true;

  unsigned long now = millis();

  // Verifica se já passou o intervalo de reconexão
  if (now - lastConnectionAttemp >= reconnectInterval) {
    lastConnectionAttemp = now;

    log(LOG_INFO, "WiFi desconectado");
    WiFi.disconnect();  // garante que está desconectado
    log(LOG_INFO, "Conectando ao wifi ");
    // WiFi.begin(cfg.wifiSSID.c_str(), cfg.wifiSSID.c_str());  // inicia reconexão
    connectToWiFi();

    // Se reconectar com sucesso, exibe informações
    if (connectToWiFi()) {
      log(LOG_INFO, "Conectado ao Wi-Fi");
      log(LOG_INFO, "IP: %s", IPparserToConstChar(WiFi.localIP()));  // mostra o IP atribuído
      return true;
    }
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
    
    snprintf(ipBuffer, sizeof(ipBuffer), "%u.%u.%u.%u",
             ip[0], ip[1], ip[2], ip[3]);
    log(LOG_INFO, "Endereço IP: %s", ipBuffer);
  }
}


/**
 * @brief Retorna se o ESP32 está conectado ao Wi-Fi.
 * 
 * @return true se conectado, false caso contrário.
 */
bool wifiIsConnected() {
  return (WiFi.status() == WL_CONNECTED);
}
