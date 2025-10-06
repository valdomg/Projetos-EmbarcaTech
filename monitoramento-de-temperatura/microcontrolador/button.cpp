#include "button.h"
#include "log.h"

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

/**
 * @brief Estado lógico do botão.
 * 
 * Essa variável é usada para armazenar o estado lógico do botão após o
 * tratamento da flag `buttonPressed`. Ela alterna (toggle) entre `true` e 
 * `false` a cada clique válido.
 */
bool stateButton = false;


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
 * - Ativa a flag `buttonPressed` quando um acionamento válido é detectado.
 * - O tempo mínimo entre interrupções aceitas é de 500 ms.
 */
void IRAM_ATTR buttonISR() {
  static unsigned long lastInterruptTime = 0;  ///< Marca o instante da última interrupção
  unsigned long currentTime = millis();

  // Verifica se passou tempo suficiente desde a última interrupção (debounce)
  if (currentTime - lastInterruptTime > 500) {
    buttonPressed = true;  // Marca que o botão foi pressionado
    log(LOG_INFO, "botao foi pressionado");
  }

  lastInterruptTime = currentTime;
}


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o botão configurando o pino como entrada com pull-up interno.
 * 
 * Esta função apenas prepara o pino, mas **não ativa a interrupção**. 
 * A ativação deve ser feita separadamente usando `enableButtonInterrupt()`.
 */
void buttonInit() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
}

/**
 * @brief Habilita a interrupção associada ao botão.
 * 
 * A ISR (`buttonISR`) será chamada sempre que o botão gerar um evento de
 * borda de subida (RISING). 
 */
void enableButtonInterrupt() {
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonISR, RISING);
}

/**
 * @brief Desabilita a interrupção associada ao botão.
 * 
 * Impede que a ISR seja chamada, útil em cenários em que não se deseja
 * que cliques do botão sejam processados temporariamente.
 */
void disableButtonInterrupt() {
  detachInterrupt(digitalPinToInterrupt(PIN_BUTTON));
}

/**
 * @brief Retorna o estado lógico atual do botão.
 * 
 * - Se a flag `buttonPressed` estiver ativa, ela é resetada.
 * - O estado lógico `stateButton` é alternado (toggle) para refletir a mudança.
 * 
 * @return true se o botão está no estado ativo.
 * @return false caso contrário.
 */
bool buttonWasPressed() {
  if (buttonPressed) {
    buttonPressed = false;
    stateButton = !stateButton;
  }
  return stateButton;
}

/**
 * @brief Reseta o estado lógico do botão.
 * 
 * Força `stateButton` para `false`, independentemente de cliques anteriores.
 * Útil para reiniciar o estado do botão em cenários de inicialização ou reset.
 */
void resetButtonState() {
  stateButton = false;
}
