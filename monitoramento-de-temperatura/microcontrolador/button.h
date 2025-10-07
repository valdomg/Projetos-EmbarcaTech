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

/**
 * @brief Inicializa o botão.
 * 
 * - Configura o pino como entrada com pull-up interno.
 * - **Não ativa a interrupção automaticamente**. 
 *   Para habilitar a interrupção deve-se chamar `enableButtonInterrupt()`.
 */
void buttonInit(uint8_t pin);

/**
 * @brief Verifica se o botão foi pressionado.
 * 
 * Essa função retorna o estado lógico do botão, atualizado 
 * pela rotina de interrupção (`muteButtonISR`).
 * 
 * - Se o botão foi pressionado, o estado lógico alterna (toggle).
 * - A flag de pressionamento é resetada internamente após a leitura.
 * 
 * @return true  se o botão está em estado ativo.
 * @return false se o botão está em estado inativo.
 */
// bool checkButtonPressed(bool *buttonPressed);

/**
 * @brief Reseta o estado lógico do botão.
 * 
 * Define o estado do botão como `false`, 
 * independente de cliques anteriores.
 */
void resetButtonState();

/**
 * @brief Desabilita a interrupção associada ao botão.
 * 
 * Após essa chamada, o botão não gerará mais eventos de interrupção
 * até que `enableButtonInterrupt()` seja chamado novamente.
 */
void disableButtonInterruptRising(uint8_t pin);

/**
 * @brief Habilita a interrupção associada ao botão.
 * 
 * Configura o microcontrolador para chamar a rotina de interrupção
 * (`muteButtonISR`) quando o botão for pressionado (borda de subida - RISING).
 */
void enableButtonInterruptRising(uint8_t pin);

void enableButtonInterruptChange(uint8_t pin);

void disableButtonInterruptChange(uint8_t pin);

bool wasMuted();

bool wasButtonLongPressed();

#endif
