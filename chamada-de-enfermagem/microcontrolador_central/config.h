#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "log.h"

constexpr LogLevel LOG_MODE = LOG_INFO;     // Configuração de modo para monitoriamento serial | opções com grau crescente de verbosidade: LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG


const constexpr uint8_t PIN_BUZZER = 15;        //pino para atuação do buzzer
const constexpr uint8_t PIN_LED = 2;           //pino para utilizaçao da led

const uint8_t NUMBER_SOUND_ALERTS = 5;

constexpr const char* MQTT_PUBLICATION_TOPIC = "dispositivos/enfermaria";
constexpr const char* MQTT_SUBSCRIPTION_TOPIC = "dispositivos/posto_enfermaria";
constexpr int MQTT_PORT = 1883;

// -----------------------------------------------------------------------------
// Constantes de configuração para rede do microcontrolador usada para acesso a configurações internas
// -----------------------------------------------------------------------------
constexpr const char* SSID_ACCESS_POINT = "xxxxxxxxxxxxx"; // Nome da Rede Wifi
constexpr const char* PASSWORD_ACCESS_POINT = "xxxxxxxxxxxx"; // Senha da Rede Wifi

#endif