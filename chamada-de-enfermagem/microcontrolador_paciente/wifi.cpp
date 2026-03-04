/**
 * @file wifi.cpp
 * @brief Implementação do módulo de gerenciamento da conexão Wi-Fi.
 *
 * @details
 * Este módulo é responsável por:
 * - Conectar o dispositivo à rede Wi-Fi;
 * - Implementar timeout de conexão;
 * - Realizar tentativas automáticas de reconexão.
 */

#include <ESP8266WiFi.h>
#include "wifi.h"

/* ============================
   Credenciais da Rede Wi-Fi
   ============================ */

/**
 * @brief SSID da rede Wi-Fi.
 */
const char* WIFI_SSID = "{id-redeWifi}";

/**
 * @brief Senha da rede Wi-Fi.
 */
const char* WIFI_PASSWORD = "{senha}";


/* ============================
   Controle de Reconexão
   ============================ */

/**
 * @brief Timestamp da última tentativa de reconexão.
 */
unsigned long lastReconnectAttempt = 0;

/**
 * @brief Intervalo entre tentativas de reconexão (ms).
 */
const unsigned long reconnectInterval = 10000;


/**
 * @brief Realiza conexão com a rede Wi-Fi.
 *
 * @return true  Se conectado com sucesso.
 * @return false Se falha na conexão.
 *
 * @details
 * Implementa timeout de 10 segundos para tentativa de conexão.
 */
bool connectWiFi() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  unsigned long timeout = millis() + 10000;

  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Conectado ao Wi-Fi");
    Serial.println(WiFi.localIP());

    return true;

  } else {

    Serial.println("Falha na conexão Wi-Fi");
    return false;
  }
}


/**
 * @brief Verifica estado da conexão Wi-Fi e tenta reconectar se necessário.
 *
 * @details
 * Executa reconexão automática caso:
 * - O Wi-Fi esteja desconectado;
 * - O intervalo mínimo entre tentativas tenha sido atingido.
 *
 * Implementação não bloqueante baseada em millis().
 */
void checkConnection() {

  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED &&
      (currentMillis - lastReconnectAttempt >= reconnectInterval)) {

    Serial.println("Wi-Fi desconectado. Tentando reconectar...");

    connectWiFi();

    lastReconnectAttempt = currentMillis;
  }
}