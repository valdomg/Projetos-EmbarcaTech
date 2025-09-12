#include "buzzer.h"
#include "log.h"

// ------------------------------------------------------------
// Variável de controle interno do módulo
// ------------------------------------------------------------

/**
 * @brief Guarda o tempo da última alternância do buzzer.
 * 
 * Usada para calcular o intervalo entre mudanças de estado,
 * evitando que o buzzer alterne em frequência muito alta.
 */
unsigned long lastBuzzerPlayTime = 0;

// ------------------------------------------------------------
// Implementação das funções públicas declaradas em buzzer.h
// ------------------------------------------------------------

void buzzerInit() {
  // Configura o pino do buzzer como saída
  // e garante que inicie desligado
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}

void toggleBuzzer(unsigned long now) {
  // Verifica se já se passou o intervalo definido
  // desde a última vez que o buzzer foi alternado
  if (now - lastBuzzerPlayTime >= BUZZER_INTERVAL_MS) {
    lastBuzzerPlayTime = now;

    // Inverte o estado atual do pino do buzzer:
    // se estava HIGH passa para LOW, e vice-versa
    digitalWrite(PIN_BUZZER, !digitalRead(PIN_BUZZER));
    log(LOG_INFO, "Buzzer alternou o estado");
  }
}

void disableSoundAlert() {
  // Desliga o buzzer forçando nível lógico baixo
  digitalWrite(PIN_BUZZER, LOW);
}
