/**
 * @file buzzer.h
 * @brief Interface do módulo de controle do buzzer.
 * 
 * Este módulo define a interface para controle do buzzer,
 * incluindo inicialização, alternância de estado e desativação
 * do alerta sonoro. O buzzer é utilizado para indicar condições
 * anormais detectadas pelo sistema de monitoramento.
 * @date 2026
 */


#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>



/**
 * @brief Intervalo de alternância do buzzer em milissegundos.
 *
 * @details
 * Define o período do alerta sonoro intermitente.
 */
constexpr unsigned long BUZZER_INTERVAL_MS = 2000;

/**
 * @brief Inicializa o buzzer configurando o pino como saída.
 * 
 * Deve ser chamada no setup() do programa antes de qualquer uso do buzzer.
 */
void buzzerInit();

/**
 * @brief Alterna o estado do buzzer respeitando o intervalo configurado.
 *
 * @param now Tempo atual em milissegundos.
 *
 * @note Deve ser chamada periodicamente no loop principal.
 */
void toggleBuzzer(unsigned long now);

/**
 * @brief Desliga o buzzer forçando nível lógico baixo no pino.
 * 
 * Deve ser chamada quando as condições de alerta não estão mais presentes.
 */
void disableSoundAlert();

#endif
