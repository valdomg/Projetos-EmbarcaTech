#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <ESP8266WiFi.h>
#include "config.h"

/**
 * @file wifi.h
 * @brief Módulo de gerenciamento da conexão Wi-Fi.
 * 
 * Este módulo fornece funções para conectar, verificar e reconectar
 * o ESP32 à rede Wi-Fi definida em config.h.
 * 
 * Funções disponíveis:
 *  - connectToWiFi()      : conecta ao Wi-Fi e retorna status.
 *  - checkAndReconnectWifi(): tenta reconectar periodicamente se desconectado.
 *  - wifiIsConnected()    : retorna true se conectado.
 */

/**
 * @brief Conecta o ESP32 à rede Wi-Fi.
 * 
 * Inicia a conexão usando SSID e senha definidos em config.h.
 * Aguarda até 10 segundos pela conexão.
 * 
 * @return true se a conexão foi bem sucedida, false caso contrário.
 */
bool connectToWiFi();

/**
 * @brief Verifica se o Wi-Fi está conectado e tenta reconectar se necessário.
 * 
 * Esta função deve ser chamada periodicamente no loop() para garantir
 * que o ESP32 tente reconectar caso a conexão seja perdida.
 * 
 * O intervalo de reconexão é definido internamente (10 segundos).
 */
bool checkAndReconnectWifi();



/**
 * @brief Cria um ponto de acesso (Access Point) para configuração local.
 * 
 * - Configura o módulo Wi-Fi no modo AP.
 * - Cria uma rede com SSID e senha pré-definidos.
 * - Exibe o endereço IP do ponto de acesso via porta serial.
 */
void createAccessPoint();

/**
 * @brief Retorna o status da conexão Wi-Fi.
 * 
 * @return true se o ESP32 estiver conectado à rede Wi-Fi, false caso contrário.
 */
bool wifiIsConnected();

#endif