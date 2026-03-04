/**
 * @file wifi.h
 * @brief Interface do módulo de gerenciamento da conexão Wi-Fi.
 *
 * @details
 * Define as funções responsáveis por:
 * - Conectar o dispositivo à rede Wi-Fi;
 * - Monitorar a conexão;
 * - Realizar tentativas automáticas de reconexão.
 */

#ifndef WIFI_H
#define WIFI_H

/**
 * @defgroup WiFiModule Módulo Wi-Fi
 * @brief Funções relacionadas à conectividade Wi-Fi.
 * @{
 */

/**
 * @brief Realiza conexão com a rede Wi-Fi configurada.
 *
 * @return true  Se conectado com sucesso.
 * @return false Se falha na conexão.
 *
 * Implementa timeout interno para tentativa de conexão.
 */
bool connectWiFi();

/**
 * @brief Verifica o estado da conexão Wi-Fi.
 *
 * Caso a conexão esteja perdida, executa tentativa
 * automática de reconexão respeitando o intervalo mínimo.
 */
void checkConnection();

/** @} */ // fim do grupo WiFiModule

#endif // WIFI_H