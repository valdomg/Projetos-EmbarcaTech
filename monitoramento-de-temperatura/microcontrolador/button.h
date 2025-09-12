#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

/**
 * @file button.h
 * @brief Declarações e definições relacionadas ao controle de botão com interrupção.
 * 
 * Este módulo gerencia um botão físico conectado ao microcontrolador,
 * incluindo inicialização do pino, detecção via interrupção e leitura do estado.
 * O debounce é tratado no arquivo `button.cpp`.
 */

/**
 * @brief Pino ao qual o botão está conectado.
 * 
 * Neste caso, o botão está ligado ao pino 13 do microcontrolador.
 * O pino é configurado com resistor de pull-up interno.
 */
constexpr uint8_t PIN_BUTTON = 13;

/**
 * @brief Inicializa o botão.
 * 
 * - Configura o pino como entrada com pull-up interno.
 * - Associa a interrupção ao pino para capturar eventos de pressionamento.
 */
void buttonInit();

/**
 * @brief Verifica se o botão foi pressionado.
 * 
 * Esta função retorna o valor da flag atualizada na interrupção.
 * 
 * @return true  se o botão foi pressionado.
 * @return false se o botão não foi pressionado.
 */
bool buttonWasPressed();

/**
 * @brief Reseta o estado do botão.
 * 
 * Usado para limpar a flag de "pressionado" depois que o evento já foi tratado.
 */
void resetButtonState();

#endif
