/**
 * @file buzzer.cpp
 * @brief Implementação do módulo de controle do buzzer.
 *
 * @details
 * Este módulo é responsável por controlar o buzzer utilizado para
 * alertas sonoros no sistema.
 *
 * O buzzer opera em modo intermitente, alternando entre ligado
 * e desligado em um intervalo de tempo definido.
 *
 * Funcionalidades:
 *
 * - Inicialização do pino
 * - Geração de alerta sonoro intermitente
 * - Desativação do alerta sonoro
 *
 * Este módulo é utilizado pelo sistema de monitoramento ambiental
 * para sinalizar condições de erro.
 *
 * @note O pino (PIN_BUZZER) e o intervalo (BUZZER_INTERVAL_MS) são definidos em config.h
 *
 * @date 2026
 */


#include "buzzer.h"
#include "log.h"
#include "config.h"


/**
 * @brief Armazena o tempo da última alternância do buzzer.
 *
 * @details
 * Utilizada para controlar o intervalo entre ativações,
 * garantindo que o buzzer opere de forma intermitente
 * sem bloquear o fluxo principal do programa.
 *
 * @note Variável de uso interno do módulo.
 */
unsigned long lastBuzzerPlayTime = 0;


/**
 * @brief Inicializa o buzzer.
 *
 * @details
 * Configura o pino do buzzer como saída digital e
 * garante que o dispositivo inicie desligado.
 *
 * Esta função deve ser chamada durante a inicialização do sistema.
 */
void buzzerInit() {
  // Configura o pino do buzzer como saída
  // e garante que inicie desligado
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}

/**
 * @brief Alterna o estado do buzzer.
 *
 * @param now Tempo atual em milissegundos (millis())
 *
 * @details
 * Alterna entre ligado e desligado em intervalos definidos,
 * criando um efeito sonoro intermitente.
 *
 * Essa abordagem evita o uso de delay(), permitindo que
 * o sistema continue executando outras tarefas.
 */
void toggleBuzzer(unsigned long now) {
  // Verifica se já se passou o intervalo definido
  // desde a última vez que o buzzer foi alternado
  if (now - lastBuzzerPlayTime >= BUZZER_INTERVAL_MS) {
    lastBuzzerPlayTime = now;

    // Inverte o estado atual do pino do buzzer:
    // se estava HIGH passa para LOW, e vice-versa
    digitalWrite(PIN_BUZZER, !digitalRead(PIN_BUZZER));
    log(LOG_DEBUG, "Buzzer alternou o estado");
  }
}

/**
 * @brief Desativa o alerta sonoro.
 *
 * @details
 * Desliga o buzzer imediatamente, colocando o pino em nível lógico baixo.
 */
void disableSoundAlert() {
  // Desliga o buzzer forçando nível lógico baixo
  digitalWrite(PIN_BUZZER, LOW);
}
