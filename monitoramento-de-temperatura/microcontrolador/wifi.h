#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

/**
 * @file wifi_helper.h
 * @brief Declarações das funções auxiliares para gerenciamento de conexão Wi-Fi.
 * 
 * Este módulo fornece funções para:
 * - Estabelecer a conexão inicial com uma rede Wi-Fi.
 * - Monitorar o status e reconectar automaticamente em caso de desconexão.
 * - Criar um ponto de acesso (Access Point) para configuração local, se necessário.
 */

/**
 * @brief Conecta o dispositivo à rede Wi-Fi configurada.
 * 
 * - Inicia a conexão utilizando o SSID e a senha definidos no módulo de configuração.
 * - Aguarda um tempo limite para o estabelecimento da conexão.
 * 
 * @return true  Se a conexão foi estabelecida com sucesso.
 * @return false Se ocorreu falha ao conectar dentro do tempo limite.
 */
bool connectWiFi();

/**
 * @brief Verifica o estado da conexão Wi-Fi e tenta reconectar quando necessário.
 * 
 * - Se o dispositivo já estiver conectado, a função não executa nenhuma ação.
 * - Caso contrário, aguarda um intervalo definido antes de tentar reconectar.
 * 
 * @return true  Se o dispositivo está conectado ou reconectado com sucesso.
 * @return false Se ainda não foi possível reconectar.
 */
bool reconnectWifi();

/**
 * @brief Cria um ponto de acesso (Access Point) para configuração local.
 * 
 * - Configura o módulo Wi-Fi no modo AP.
 * - Cria uma rede com SSID e senha pré-definidos.
 * - Exibe o endereço IP do ponto de acesso via porta serial.
 */
void createAccessPoint();

#endif
