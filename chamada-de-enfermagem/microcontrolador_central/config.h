#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include "log.h"

constexpr LogLevel LOG_MODE = LOG_INFO;     // Configuração de modo para monitoriamento serial | opções com grau crescente de verbosidade: LOG_NONE, LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG


const constexpr uint8_t PIN_BUZZER = 15;        //pino para atuação do buzzer
const constexpr uint8_t PIN_LED = 2;           //pino para utilizaçao da led

const uint8_t NUMBER_SOUND_ALERTS = 5;

constexpr const char* WIFI_SSID = "xxxxxxxxxxxxxx";
constexpr const char* WIFI_PASSWORD = "xxxxxxxxxxxxxx";


constexpr const char* MQTT_SERVER = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
constexpr const char* MQTT_USER = "xxxxxxxxxxxxxx";
constexpr const char* MQTT_PASS = "xxxxxxxxxxxxxx";
constexpr const char* MQTT_DEVICE_ID = "xxxxxxxxxxxxxx";
constexpr const char* MQTT_PUB_CONFIRMATION_TOPIC = "dispositivo/confirmacao";
constexpr const char* MQTT_SUB_CONFIRMATION_TOPIC = "dispositivo/confirmacao/posto_enfermaria";
constexpr  const char* MQTT_PUBLICATION_TOPIC = "dispositivos/enfermaria";
constexpr  const char* MQTT_SUBSCRIPTION_TOPIC = "dispositivos/posto_enfermaria";
constexpr  int MQTT_PORT = 1883;

#endif