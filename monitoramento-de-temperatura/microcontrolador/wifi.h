#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

/**
 * @file wifi_helper.h
 * @brief Declarações das funções auxiliares para gerenciamento de conexão Wi-Fi.
 * 
 * Este módulo fornece funções para:
 * - Estabelecer conexão inicial com uma rede Wi-Fi.
 * - Reestabelecer a conexão automaticamente em caso de queda.
 */

/**
 * @brief Conecta à rede Wi-Fi configurada.
 * 
 * - Inicia a conexão com SSID e senha definidos no módulo de implementação.
 * - Aguarda um tempo limite para confirmar a conexão.
 * 
 * @return true  Conexão estabelecida com sucesso.
 * @return false Falha ao conectar dentro do tempo limite.
 */
bool connectWiFi();

/**
 * @brief Verifica e reconecta ao Wi-Fi se necessário.
 * 
 * - Se o dispositivo já estiver conectado, não faz nada.
 * - Caso contrário, tenta reconectar em intervalos definidos.
 */
bool reconnectWifi();


#endif
