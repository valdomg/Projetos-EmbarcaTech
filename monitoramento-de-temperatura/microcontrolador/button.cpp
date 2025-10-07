#include "c_types.h"
#include "button.h"
#include "log.h"
#include "config.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

volatile unsigned long buttonPressStartTime = 0;
volatile bool flagLongPress = false;
volatile bool isButtonPressed = false;


/**
 * @brief Flag que indica se o botão foi pressionado.
 * 
 * É declarada como `volatile` porque pode ser modificada dentro da ISR
 * (rotina de interrupção), garantindo que o compilador não otimize de forma
 * incorreta o acesso a essa variável.
 */
volatile bool flagMutePressed = false;

/**
 * @brief Estado lógico do botão.
 * 
 * Essa variável é usada para armazenar o estado lógico do botão após o
 * tratamento da flag `buttonPressed`. Ela alterna (toggle) entre `true` e 
 * `false` a cada clique válido.
 */
bool isMuted = false;



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
void IRAM_ATTR muteButtonISR() {
  static unsigned long lastInterruptTime = 0;  ///< Marca o instante da última interrupção
  unsigned long currentTime = millis();

  // Verifica se passou tempo suficiente desde a última interrupção (debounce)
  if (currentTime - lastInterruptTime > 500) {
    flagMutePressed = true;  // Marca que o botão foi pressionado
  }

  lastInterruptTime = currentTime;
}


void IRAM_ATTR longPressButtonISR() {  //funçao vai calcular quantos segundos o botao foi pressionado

  bool readButton = digitalRead(PIN_BUTTON_LONG);


  if (readButton == LOW && !isButtonPressed) {

    buttonPressStartTime = millis();
    isButtonPressed = true;

  } else if (readButton == HIGH && isButtonPressed) {

    if (millis() - buttonPressStartTime  > 15 * 1000) {
      flagLongPress = !flagLongPress;
    }
    isButtonPressed = false;
  }
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
void buttonInit(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
}

/**
 * @brief Habilita a interrupção associada ao botão.
 * 
 * A ISR (`muteButtonISR`) será chamada sempre que o botão gerar um evento de
 * borda de subida (RISING). 
 */
void enableButtonInterruptRising(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), muteButtonISR, RISING);
}

/**
 * @brief Desabilita a interrupção associada ao botão.
 * 
 * Impede que a ISR seja chamada, útil em cenários em que não se deseja
 * que cliques do botão sejam processados temporariamente.
 */
void disableButtonInterruptRising(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}
/**
 * @brief Habilita a interrupção associada ao botão.
 * 
 * A ISR (``) será chamada sempre que o botão gerar um evento de mudança (CHANGE). 
 */
void enableButtonInterruptChange(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), longPressButtonISR, CHANGE);
}

/**
 * @brief Desabilita a interrupção associada ao botão.
 * 
 * Impede que a ISR seja chamada, útil em cenários em que não se deseja
 * que cliques do botão sejam processados temporariamente.
 */
void disableButtonInterruptChange(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

/**
 * @brief Retorna o estado lógico atual do botão.
 * 
 * - Se a flag `buttonPressed` estiver ativa, ela é resetada.
 * - O estado lógico `isMuted` é alternado (toggle) para refletir a mudança.
 * 
 * @return true se o botão está no estado ativo.
 * @return false caso contrário.
 */
bool checkButtonPressed(volatile bool *buttonPressed) {
  if (*buttonPressed) {

    *buttonPressed = false;
    return true;  // apenas indica que houve um clique
  }
  return false;
}


bool wasMuted() {

  if (checkButtonPressed(&flagMutePressed)) {
    isMuted = !isMuted;
  }

  return isMuted;
}


bool wasButtonLongPressed() {
  return flagLongPress;
}



/**
 * @brief Reseta o estado lógico do botão.
 * 
 * Força `isMuted` para `false`, independentemente de cliques anteriores.
 * Útil para reiniciar o estado do botão em cenários de inicialização ou reset.
 */
void resetButtonState() {
  isMuted = false;
}
