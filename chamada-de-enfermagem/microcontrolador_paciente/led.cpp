#include "led.h"
#include <Arduino.h>

const int LED = 4;

void setupLed() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void ligarLed() {
  digitalWrite(LED, HIGH);
}

void desligarLed() {
  digitalWrite(LED, LOW);
}