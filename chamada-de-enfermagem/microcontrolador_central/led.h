/**
 * @file led.h
 * @brief Interface para controle do LED indicador do sistema.
 *
 * @details
 * Este módulo define a interface pública para controle do LED conectado
 * ao microcontrolador. Ele fornece funções simples para inicialização
 * do hardware, acionamento direto do LED e operação em modo intermitente
 * (piscar).
 *
 * O comportamento de piscar do LED é implementado no arquivo `led.cpp`
 * utilizando temporização baseada na função `millis()`, evitando o uso
 * de `delay()` e permitindo execução não bloqueante do firmware.
 *
 * Funcionalidades disponíveis:
 * - Inicializar o pino do LED
 * - Ligar o LED
 * - Desligar o LED
 * - Alternar o estado do LED periodicamente
 *
 * O pino utilizado para controle do LED é definido no arquivo `config.h`
 *
 * @see led.cpp
 * @see config.h
 * @date 2026
 */
#ifndef LED_H
#define LED_H

#include <Arduino.h>

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
 */
void toggleLed();

/**
 * @brief Liga o LED (força nível lógico HIGH no pino).
 */
void turnOnLed();

/**
 * @brief Desliga o LED (força nível lógico LOW no pino).
 */
void turnOffLed();

#endif
