/**
 * @file wifi_utils.cpp
 * @brief Implementação do módulo de gerenciamento de conexão Wi-Fi do dispositivo.
 *
 * @details
 * Este módulo é responsável por controlar toda a conectividade Wi-Fi do sistema.
 * Ele fornece funções para:
 *
 * - conectar o dispositivo a uma rede Wi-Fi existente
 * - monitorar o estado da conexão
 * - realizar reconexão automática quando necessário
 * - criar um Access Point para configuração do dispositivo
 *
 * O módulo também exibe o endereço IP obtido no display LCD do sistema
 * quando a conexão é estabelecida.
 *
 * ## Fluxo de funcionamento
 *
 * 1. O sistema tenta conectar à rede Wi-Fi utilizando `connectToWiFi()`.
 * 2. Durante a execução normal, `checkAndReconnectWifi()` monitora a conexão.
 * 3. Caso a conexão seja perdida, o sistema tenta reconectar automaticamente.
 * 4. Se não houver rede disponível ou o dispositivo estiver em modo de
 *    configuração, `createAccessPoint()` cria um ponto de acesso local.
 *
 * ## Dependências
 *
 * Este módulo utiliza:
 *
 * - ESP8266WiFi → gerenciamento da interface Wi-Fi
 * - config_storage → leitura das configurações de rede
 * - log → registro de eventos do sistema
 * - display_LCD-2004_I2C → exibição do endereço IP no display
 */

#include <ESP8266WiFi.h>  // Biblioteca oficial ESP32 para conexão Wi-Fi
#include "wifi_utils.h"   // Declarações das funções públicas do módulo Wi-Fi
#include "config.h"       // Constantes globais do projeto (SSID e senha)
#include "log.h"
#include "config_storage.h"

#include "display_LCD-2004_I2C.h"  // Para mostrar o IP da rede


// ------------------------------------------------------------
// Variáveis internas do módulo
// ------------------------------------------------------------

/**
 * @brief Buffer utilizado para armazenar o endereço IP em formato string.
 *
 * Exemplo de valor armazenado:
 * `"192.168.1.10"`
 */
char ipBuffer[16];  

// ------------------------------------------------------------
// Funções auxiliares internas
// ------------------------------------------------------------

/**
 * @brief Converte um objeto IPAddress para uma string constante.
 *
 * @param ip Endereço IP a ser convertido.
 *
 * @return Ponteiro para string contendo o endereço IP no formato
 *         `"xxx.xxx.xxx.xxx"`.
 */
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
 * @brief Intervalo mínimo entre tentativas de reconexão Wi-Fi.
 *
 * Valor em milissegundos utilizado para evitar reconexões muito
 * frequentes quando a rede está instável.
 */
static const unsigned long reconnectInterval = 2000;  // 2 segundos

// ------------------------------------------------------------
// Implementação das funções públicas do módulo
// ------------------------------------------------------------

/**
 * @brief Conecta o dispositivo à rede Wi-Fi configurada.
 *
 * A função utiliza as credenciais armazenadas em `cfg.wifiSSID`
 * e `cfg.wifiPass`.
 *
 * O sistema aguarda um tempo limitado pela conexão e exibe
 * o endereço IP obtido no display LCD caso a conexão seja
 * bem sucedida.
 *
 * @return true se a conexão foi estabelecida com sucesso.
 * @return false caso a conexão falhe ou expire o tempo limite.
 */
bool connectToWiFi() {
  WiFi.begin(cfg.wifiSSID.c_str(), cfg.wifiPass.c_str());  // inicia conexão
  log(LOG_INFO, "Conctandoo ao wifi");

  // Define um timeout de 10 segundos
  unsigned long timeout = millis() + 5000;

  // Aguarda até a conexão ou até o timeout
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);         // aguarda meio segundo entre tentativas
    Serial.print(".");  // imprime ponto para indicar progresso
  }

  Serial.println();

  // Verifica se a conexão foi bem sucedida
  if (WiFi.status() == WL_CONNECTED) {


    // Mostrar no display o IP da rede
    showIPAddress(IPparserToConstChar(WiFi.localIP()));
    return true;
  }

  log(LOG_ERROR, "Falha na conexão Wi-Fi");
  return false;
}

/**
 * @brief Verifica o estado da conexão Wi-Fi e tenta reconectar se necessário.
 *
 * Esta função deve ser chamada periodicamente dentro do `loop()`
 * principal do firmware.
 *
 * Caso o dispositivo esteja desconectado da rede, uma nova tentativa
 * de conexão será realizada respeitando o intervalo definido em
 * `reconnectInterval`.
 *
 * @return true se o dispositivo estiver conectado ao Wi-Fi.
 * @return false caso continue desconectado.
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
 * @brief Cria um Access Point para configuração do dispositivo.
 *
 * Caso o dispositivo não esteja operando em modo Access Point,
 * esta função:
 *
 * - Desconecta a interface Wi-Fi atual
 * - Define o modo `WIFI_AP`
 * - Cria um Access Point com SSID e senha definidos nas constantes
 *   `SSID_ACCESS_POINT` e `PASSWORD_ACCESS_POINT`
 *
 * O endereço IP do Access Point também é exibido no display LCD.
 */
void createAccessPoint() {
  WiFiMode_t mode = WiFi.getMode();

  if (mode != WIFI_AP) {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID_ACCESS_POINT, PASSWORD_ACCESS_POINT);
    IPAddress ip = WiFi.softAPIP();

    showIPAddress(IPparserToConstChar(ip));
    log(LOG_INFO, "Endereço IP: %s", IPparserToConstChar(ip));
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
