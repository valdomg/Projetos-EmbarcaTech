/**
 * @file led.cpp
 * @brief Implementação do módulo de controle do LED indicador.
 * 
 * @details
 * Este módulo é responsável por configurar e controlar o LED
 * utilizado como indicador visual no sistema de chamada de enfermagem.
 */

#include "led.h"
#include <Arduino.h>

/**
 * @brief Pino digital onde o LED está conectado.
 */
const int LED = 4;

/**
 * @brief Inicializa o LED indicador.
 * 
 * Configura o pino como saída digital e garante que o LED
 * inicie desligado.
 */
void setupLed() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

/**
 * @brief Liga o LED indicador.
 * 
 * Define o pino em nível lógico HIGH.
 */
void ligarLed() {
  digitalWrite(LED, HIGH);
}

/**
 * @brief Desliga o LED indicador.
 * 
 * Define o pino em nível lógico LOW.
 */
void desligarLed() {
  digitalWrite(LED, LOW);
}