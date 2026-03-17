/**
 * @file button.h
 * @brief Interface do módulo de controle do botão do paciente.
 * 
 * @details
 * Este módulo define as funções responsáveis pela configuração
 * e leitura do botão físico utilizado no sistema de chamada de enfermagem.
 */

#ifndef BUTTON_H
#define BUTTON_H

/**
 * @defgroup ButtonModule Módulo do Botão
 * @brief Funções e definições relacionadas ao botão do paciente.
 * @{
 */

/**
 * @brief Pino digital onde o botão está conectado.
 * 
 * Definido no arquivo button.cpp.
 */
extern const int BUTTON;

/**
 * @brief Inicializa o botão do paciente.
 * 
 * Configura o pino como entrada com resistor de pull-up interno.
 */
void setupButton();

/**
 * @brief Lê o estado atual do botão.
 * 
 * @return true  Nível lógico HIGH (botão solto).
 * @return false Nível lógico LOW (botão pressionado).
 */
bool readButton();

/** @} */ // fim do grupo ButtonModule

#endif // BUTTON_H