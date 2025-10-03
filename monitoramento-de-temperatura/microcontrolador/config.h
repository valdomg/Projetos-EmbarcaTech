#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// -----------------------------------------------------------------------------
// Constantes de configuração dos pinos
// -----------------------------------------------------------------------------
const constexpr uint8_t PIN_BUTTON_MUTE = 13;   // botão de mute
const constexpr uint8_t PIN_BUTTON_LONG = 15;   // botão long press

const constexpr uint8_t PIN_SENSOR_SCL = 5;     //pino scl do sensor aht10
const constexpr uint8_t PIN_SENSOR_SDA = 4;     //pino sda do sensor aht10

const constexpr uint8_t PIN_BUZZER = 14;        //pino para atuação do buzzer

const constexpr uint8_t PIN_LED = 12;           //pino para utilizaçao da led


// -----------------------------------------------------------------------------
// Constantes de configuração do MQTT
// -----------------------------------------------------------------------------

/// Endereço do broker MQTT.
constexpr char* MQTT_SERVER = "xxxxxxxxxxx";

/// Usuário para autenticação no broker MQTT.
constexpr char* MQTT_USER = "xxxxxxxxxxxx";

/// Senha para autenticação no broker MQTT.
constexpr char* MQTT_PASS = "xxxxxxxxxxxx";

/// Tópico para publicação de dados de sensores (temperatura/umidade).
constexpr char* MQTT_TOPIC_PUBLICATION_DATA = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Tópico para publicação de alertas.
constexpr char* MQTT_TOPIC_PUBLICATION_ALERT = "xxxxxxxxxxxx/xxxxxxxxxxxx";

/// Identificação única do dispositivo no broker MQTT.
constexpr char* MQTT_DEVICE_ID = "xxxxxxxxxxxx";


#endif