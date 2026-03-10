/**
 * @file wifi_utils.h
 * @brief Interface do módulo de gerenciamento de conexão Wi-Fi.
 *
 * @details
 * Este módulo fornece funções para controle da conectividade Wi-Fi
 * do dispositivo. Ele permite:
 *
 * - Conectar o dispositivo a uma rede Wi-Fi configurada
 * - Monitorar o estado da conexão
 * - Realizar reconexão automática em caso de perda de conexão
 * - Criar um Access Point para configuração local
 *
 * Essas funções são utilizadas pelo firmware principal para garantir
 * que o dispositivo permaneça conectado à rede e possa se comunicar
 * com outros serviços (como MQTT).
 *
 * ## Fluxo típico de uso
 *
 * 1. O sistema tenta conectar à rede com `connectToWiFi()`.
 * 2. Durante a execução normal, `checkAndReconnectWifi()` é chamada
 *    no `loop()` principal para verificar a conexão.
 * 3. Caso não seja possível conectar à rede, o sistema pode criar
 *    um Access Point usando `createAccessPoint()` para permitir
 *    configuração local do dispositivo.
 *
 * @note
 * Este módulo depende da biblioteca `ESP8266WiFi`.
 */

#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <ESP8266WiFi.h>
#include "config.h"


/**
 * @brief Conecta o dispositivo à rede Wi-Fi configurada.
 *
 * Utiliza as credenciais armazenadas na estrutura de configuração
 * (`cfg.wifiSSID` e `cfg.wifiPass`).
 *
 * A função aguarda um tempo limitado pela conexão e retorna
 * o resultado da tentativa.
 *
 * @return true se a conexão foi estabelecida com sucesso.
 * @return false se a conexão falhar.
 */
bool connectToWiFi();

/**
 * @brief Verifica o estado da conexão Wi-Fi e tenta reconectar.
 *
 * Caso o dispositivo esteja desconectado da rede, a função
 * aguarda o intervalo configurado internamente antes de tentar
 * uma nova conexão.
 *
 * Essa função deve ser chamada periodicamente no `loop()` principal.
 *
 * @return true se o dispositivo estiver conectado ao Wi-Fi.
 * @return false se continuar desconectado.
 */
bool checkAndReconnectWifi();



/**
 * @brief Cria um Access Point para configuração do dispositivo.
 *
 * Esta função coloca o dispositivo em modo `WIFI_AP` e cria
 * um ponto de acesso utilizando o SSID e senha definidos
 * nas constantes do sistema.
 *
 * O endereço IP do Access Point pode ser utilizado pelo
 * usuário para acessar a interface web de configuração.
 */
void createAccessPoint();

/**
 * @brief Verifica se o dispositivo está conectado à rede Wi-Fi.
 *
 * @return true se o dispositivo estiver conectado.
 * @return false caso contrário.
 */
bool wifiIsConnected();


const char* getIPwifi();

#endif