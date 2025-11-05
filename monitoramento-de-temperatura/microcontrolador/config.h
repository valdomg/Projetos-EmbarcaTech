#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// -----------------------------------------------------------------------------
// Constantes de configuração dos pinos
// -----------------------------------------------------------------------------
const constexpr uint8_t PIN_BUTTON_MUTE = 13;   // botão de mute
const constexpr uint8_t PIN_BUTTON_LONG = 2;   // botão long press

const constexpr uint8_t PIN_SENSOR_SCL = 5;     //pino scl do sensor aht10
const constexpr uint8_t PIN_SENSOR_SDA = 4;     //pino sda do sensor aht10

const constexpr uint8_t PIN_BUZZER = 0;        //pino para atuação do buzzer

const constexpr uint8_t PIN_LED = 12;           //pino para utilizaçao da led


// -----------------------------------------------------------------------------
// Constantes de configuração para rede do microcontrolador usada para acesso a configurações internas
// -----------------------------------------------------------------------------
constexpr const char* SSID_ACCESS_POINT = "xxxxxxxxxxxxx"; // Nome da Rede Wifi
constexpr const char* PASSWORD_ACCESS_POINT = "xxxxxxxxxxxx"; // Senha da Rede Wifi


#endif