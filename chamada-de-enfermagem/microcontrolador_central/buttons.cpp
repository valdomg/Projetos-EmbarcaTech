#include "buttons.h"

// debounce por software para evitar múltiplas leituras de um mesmo pressionamento
constexpr unsigned long DEBOUNCE_DELAY = 50;

// ===== Instâncias dos Botões =====
Button button_next   = {14, HIGH, HIGH, 0}; // recebe o pino e estado inicial (HIGH)
Button button_prev   = {12, HIGH, HIGH, 0}; // recebe o pino e estado inicial (HIGH)
Button button_delete = {13, HIGH, HIGH, 0}; // recebe o pino e estado inicial (HIGH)


// ===== Inicialização dos botões =====
void initButtons() {
  // Configura os pinos como entrada com pull-up interno (LOW quando pressionado)
  pinMode(button_next.pin, INPUT_PULLUP);  
  pinMode(button_prev.pin, INPUT_PULLUP);
  pinMode(button_delete.pin, INPUT_PULLUP);
}


// ===== Debounce =====
bool checkButton(Button &btn) {
  uint8_t reading = digitalRead(btn.pin);

  if (reading != btn.lastRawReading) { // Reinicia o timer se a leitura muda
    btn.lastDebounceTime = millis();
  }

  // Atualiza o estado debounced somente se permaneceu estável por DEBOUNCE_DELAY ms
  if ((millis() - btn.lastDebounceTime) >= DEBOUNCE_DELAY) {
    if (reading != btn.state) {
      btn.state = reading;

      // Retorna true apenas quando detecta pressão do botão (LOW)
      if (btn.state == LOW) {
        btn.lastRawReading = reading;
        return true;
      }
    }
  }

  btn.lastRawReading = reading;
  return false;
}