#ifndef LED_H
#define LED_H

#include <Arduino.h>   // Necessário para usar funções e tipos da plataforma Arduino

// -----------------------------------------------------------------------------
// Definições de pinos e constantes
// -----------------------------------------------------------------------------

/**
 * @brief Pino digital utilizado para o controle do LED.
 * 
 * Neste caso, o LED está conectado ao pino 12 do microcontrolador.
 */
constexpr uint8_t PIN_LED = 12;


// -----------------------------------------------------------------------------
// Protótipos das funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o LED configurando o pino como saída.
 * 
 * Deve ser chamada uma vez no setup() para preparar o hardware.
 */
void ledInit();

/**
 * @brief Alterna o estado do LED em intervalos regulares.
 * 
 * A lógica de controle do tempo utiliza millis() para evitar delays bloqueantes.
 * 
 * @param now Tempo atual em milissegundos (obtido com millis()).
 */
void toggleLed(unsigned long now);

/**
 * @brief Liga o LED (força nível lógico HIGH no pino).
 */
void turnOnLed();

/**
 * @brief Desliga o LED (força nível lógico LOW no pino).
 */
void turnOffLed();

#endif
