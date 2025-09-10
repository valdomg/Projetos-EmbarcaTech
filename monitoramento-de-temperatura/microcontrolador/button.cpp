#include "button.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Flag que indica se o botão foi pressionado.
 * 
 * É declarada como `volatile` porque pode ser modificada dentro da ISR
 * (rotina de interrupção), garantindo que o compilador não otimize de forma
 * incorreta o acesso a essa variável.
 */
volatile bool buttonPressed = false;


// -----------------------------------------------------------------------------
// Rotina de interrupção (ISR)
// -----------------------------------------------------------------------------

/**
 * @brief Rotina chamada automaticamente quando ocorre a interrupção do botão.
 * 
 * Esta função é atribuída ao pino do botão via `attachInterrupt()`. 
 * Implementa um mecanismo de debounce por software para evitar múltiplas 
 * detecções de um único clique.
 * 
 * - O estado de `buttonPressed` é alternado (toggle) a cada acionamento válido.
 * - O tempo mínimo entre interrupções aceitas é de 500 ms.
 */
void IRAM_ATTR buttonISR() {
  static unsigned long lastInterruptTime = 0;  // Armazena o último instante da interrupção
  unsigned long currentTime = millis();

  // Verifica se passou tempo suficiente desde a última interrupção (debounce)
  if (currentTime - lastInterruptTime > 500) {
    buttonPressed = true;  // Alterna o estado da flag
  }

  lastInterruptTime = currentTime;
}


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o botão configurando o pino e a interrupção.
 * 
 * - O pino é configurado como entrada com pull-up interno.
 * - A interrupção é configurada para disparar na borda de subida (RISING).
 * - Essa função deve ser chamada no setup() antes de usar o botao.
 */
void buttonInit() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonISR, RISING);
}

/**
 * @brief Retorna o estado atual da flag do botão.
 * 
 * @return true se o botão foi pressionado (flag ativa).
 * @return false caso contrário.
 */
bool buttonWasPressed() {
    if (buttonPressed) {
    buttonPressed = false;
    return true;
  }
  return false;
  
}

/**
 * @brief Reseta o estado do botão, limpando a flag de pressionado.
 */
void resetButtonState() {
  buttonPressed = false;
}
