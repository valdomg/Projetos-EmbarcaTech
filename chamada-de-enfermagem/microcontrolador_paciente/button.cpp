/**
 * @file button.cpp
 * @brief Implementação do módulo de leitura do botão do paciente.
 * 
 * @details
 * Este módulo é responsável por configurar e realizar a leitura
 * do botão físico utilizado para acionamento da chamada de enfermagem.
 */

#include "button.h"
#include <Arduino.h>

/**
 * @brief Pino digital onde o botão está conectado.
 * 
 * Configurado como INPUT_PULLUP.
 */
const int BUTTON = 5;

/**
 * @brief Inicializa o botão do paciente.
 * 
 * Configura o pino definido como entrada com resistor de pull-up interno.
 * O botão deve estar ligado entre o pino e o GND.
 */
void setupButton(){
  pinMode(BUTTON, INPUT_PULLUP);
}

/**
 * @brief Realiza a leitura do estado do botão.
 * 
 * @return true  Botão em nível HIGH (não pressionado, devido ao pull-up).
 * @return false Botão em nível LOW (pressionado).
 * 
 * @note Como o pino está configurado com INPUT_PULLUP, o estado lógico é invertido:
 * - LOW  → botão pressionado
 * - HIGH → botão solto
 */
bool readButton() {
  return digitalRead(BUTTON);
}