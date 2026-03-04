/**
 * @file led.h
 * @brief Interface do módulo de controle do LED indicador.
 * 
 * @details
 * Este módulo fornece as funções responsáveis pela configuração
 * e controle do LED utilizado como sinalização visual no sistema
 * de chamada de enfermagem.
 */

#ifndef LED_H
#define LED_H

/**
 * @defgroup LedModule Módulo do LED
 * @brief Funções e definições relacionadas ao LED indicador.
 * @{
 */

/**
 * @brief Pino digital onde o LED está conectado.
 * 
 * Definido no arquivo led.cpp.
 */
extern const int LED;

/**
 * @brief Inicializa o LED indicador.
 * 
 * Configura o pino como saída digital e garante que o LED
 * inicie no estado desligado.
 */
void setupLed();

/**
 * @brief Liga o LED indicador.
 */
void ligarLed();

/**
 * @brief Desliga o LED indicador.
 */
void desligarLed();

/** @} */ // fim do grupo LedModule

#endif // LED_H