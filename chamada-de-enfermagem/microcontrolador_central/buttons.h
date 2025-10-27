#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

// ===== Estrutura do botão =====
struct Button {
  uint8_t pin; // pino do microcontrolador onde o botão está conectado
  uint8_t state;  // estado debounced atual do botão (LOW = pressionado, HIGH = não pressionado)
  uint8_t lastRawReading; // última leitura não debounced do pino
  unsigned long lastDebounceTime; // instante em que a última mudança de leitura bruta foi detectada
};


// ===== Instâncias dos Botões =====
extern Button button_next;
extern Button button_prev;
extern Button button_delete;

extern void initButtons();
extern bool checkButton(Button &btn);

#endif