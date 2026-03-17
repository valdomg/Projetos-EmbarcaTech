/**
 * @file config.h
 * @brief Definições globais de hardware e parâmetros de configuração do sistema.
 *
 * @details
 * Este arquivo centraliza todas as constantes utilizadas para:
 *
 * - Mapeamento de pinos do microcontrolador
 * - Configuração do modo Access Point (AP)
 *
 * Isso facilita manutenção, portabilidade e organização do firmware.
 *
 * @note
 * Sempre que houver alteração de hardware, este arquivo deve ser atualizado.
 *
 * @warning
 * Evite modificar esses valores em tempo de execução.
 * Eles representam a configuração física do dispositivo.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>



// ============================================================================
// Mapeamento de pinos
// ============================================================================

/**
 * @brief Pino do botão de mute.
 *
 * Utilizado para ativar/desativar o som do sistema.
 *
 * Evento detectado via interrupção.
 */
constexpr uint8_t PIN_BUTTON_MUTE = 13;


/**
 * @brief Pino do botão de pressão longa.
 *
 * Utilizado para comandos especiais, como:
 *
 * - Reset
 * - Modo configuração
 */
constexpr uint8_t PIN_BUTTON_LONG = 2;


/**
 * @brief Pino SCL do sensor AHT10 (I2C Clock).
 */
constexpr uint8_t PIN_SENSOR_SCL = 5;


/**
 * @brief Pino SDA do sensor AHT10 (I2C Data).
 */
constexpr uint8_t PIN_SENSOR_SDA = 4;


/**
 * @brief Pino de controle do buzzer.
 *
 * Utilizado para alertas sonoros.
 */
constexpr uint8_t PIN_BUZZER = 12;


/**
 * @brief Pino do LED de status.
 *
 * Utilizado para indicar estados do sistema.
 */
constexpr uint8_t PIN_LED = 0;



// ============================================================================
// Configuração do Access Point
// ============================================================================

/**
 * @brief Nome da rede Wi-Fi criada pelo dispositivo (Access Point).
 *
 * @details
 * Esta rede é utilizada para acessar o portal de configuração do dispositivo.
 *
 * Exemplo:
 *
 * Device_Config
 */
constexpr const char* SSID_ACCESS_POINT = "xxxxxxxxxxxxx";



/**
 * @brief Senha da rede Wi-Fi criada pelo dispositivo.
 *
 * @warning
 * Recomenda-se utilizar senha forte em ambiente de produção.
 */
constexpr const char* PASSWORD_ACCESS_POINT = "xxxxxxxxxxxx";


#endif