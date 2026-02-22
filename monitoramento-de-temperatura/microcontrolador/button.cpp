/**
 * @file button.cpp
 * @brief Implementação do módulo de controle de botões físicos.
 *
 * @details
 * Este módulo é responsável pelo gerenciamento de dois botões físicos
 * utilizados no sistema:
 *
 * - Botão de mute (pressionamento curto)
 * - Botão de long press (pressionamento longo)
 *
 * Funcionalidades implementadas:
 *
 * - Detecção de pressionamento curto
 * - Detecção de pressionamento longo
 * - Alternância de estado lógico (toggle)
 * - Tratamento de debounce por software
 * - Uso de interrupções externas (ISR)
 *
 * O módulo utiliza variáveis do tipo volatile para garantir integridade
 * no acesso concorrente entre o código principal e as rotinas de interrupção.
 *
 * @note Este módulo depende das definições de pinos em config.h
 *
 * @date 2026
 */

#include "c_types.h"
#include "button.h"
#include "log.h"
#include "config.h"

/**
 * @brief Armazena o instante em que o botão foi pressionado.
 *
 * @details
 * Utilizado para calcular o tempo de duração do pressionamento
 * e determinar se foi um short press ou long press.
 *
 * @note Variável modificada dentro de ISR.
 */
volatile unsigned long buttonPressStartTime = 0;

/**
 * @brief Flag que indica ocorrência de long press.
 *
 * @details
 * É ativada quando o botão permanece pressionado por tempo
 * superior ao limite definido.
 *
 * @note Acessada no loop principal.
 */
volatile bool flagLongPress = false;


/**
 * @brief Flag que indica ocorrência de short press.
 *
 * @note Definida dentro da ISR e consumida no loop principal.
 */
volatile bool flagShortPress = false;


/**
 * @brief Indica se o botão está atualmente pressionado.
 *
 * @details
 * Utilizado para controlar a transição de estados
 * impedindo a atualizacao do buttonPressStartTime
 * para estado atual e obtendo o momento em que o botao foi 
 * precionado.
 */
volatile bool isButtonPressed = false;


/**
 * @brief Flag de pressionamento do botão mute.
 *
 * @details
 * Utilizada para sinalizar que ocorreu um clique válido,
 * após tratamento de debounce.
 */
volatile bool flagMutePressed = false;


bool isLCDturnOn = false;


/**
 * @brief Indica se o sistema está em modo mute.
 *
 * @details
 * Alterna seu valor a cada clique válido no botão mute.
 */
bool isMuted = false;


// -----------------------------------------------------------------------------
// Rotinas de interrupção (ISR)
// -----------------------------------------------------------------------------

/**
 * @brief Rotina de interrupção do botão de mute.
 *
 * @details
 * Executada automaticamente quando ocorre uma borda de subida
 * no pino configurado.
 *
 * Implementa debounce por software para evitar múltiplas
 * detecções causadas por ruído mecânico.
 *
 * @note Esta função é executada em contexto de interrupção.
 */
void IRAM_ATTR muteButtonISR() {
  static unsigned long lastInterruptTime = 0;  ///< Marca o instante da última interrupção
  unsigned long currentTime = millis();

  if (currentTime - lastInterruptTime > 500) {
    flagMutePressed = true;  // Marca que o botão foi pressionado
  }

  lastInterruptTime = currentTime;
}

/**
 * @brief Rotina de interrupção para detecção de long press.
 *
 * @details
 * Monitora o tempo em que o botão permanece pressionado.
 *
 * Comportamento:
 *
 * - Se pressionado por mais de 15 segundos → long press
 * - Se pressionado entre 100 ms e 15 segundos → short press
 *
 * @note Executada em contexto de interrupção.
 */
void IRAM_ATTR longPressButtonISR() {
  bool readButton = digitalRead(PIN_BUTTON_LONG);

  if (readButton == LOW && !isButtonPressed) { //captura o tempo inicial onde o botao foi precionado evento de decida
    // Início da pressão                       //impede que quando o botao matem-se pressionado o registro de tempo atualize novamente
    buttonPressStartTime = millis();
    isButtonPressed = true;

  } else if (readButton == HIGH && isButtonPressed) { // checa se o botao foi solto
    // Soltou o botão
    unsigned long now = millis();
    if (now - buttonPressStartTime >= 15 * 1000) { //verifica se o botao passou 15 segundos pressionados
      flagLongPress = !flagLongPress;
    } else if (now - buttonPressStartTime < 15 * 1000 && now - buttonPressStartTime >= 100) { //verifica se foi um short press
      flagShortPress = true;
      
    }
    isButtonPressed = false;
  }
}


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o pino do botão.
 *
 * @param pin Número do pino digital conectado ao botão.
 *
 * @details
 * Configura o pino como entrada com resistor pull-up interno.
 *
 * @note Deve ser chamada durante o setup do sistema.
 */
void buttonInit(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
}

/**
 * @brief Habilita interrupção por borda de subida.
 *
 * @param pin Pino do botão.
 *
 * @details
 * Associa o pino à ISR responsável pelo tratamento de
 * pressionamento do botão mute.
 */
void enableButtonInterruptRising(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), muteButtonISR, RISING);
}

/**
 * @brief Desabilita interrupção por borda de subida.
 *
 * @param pin Pino do botão.
 *
 * @details
 * Associa o pino à ISR responsável pelo tratamento de
 * pressionamento do botão mute.
 */
void disableButtonInterruptRising(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

/**
 * @brief Habilita interrupção por mudança de estado.
 *
 * @param pin Pino do botão.
 *
 * @details
 * Utilizada para detectar início e fim do pressionamento,
 * permitindo calcular sua duração.
 */
void enableButtonInterruptChange(uint8_t pin) {
  attachInterrupt(digitalPinToInterrupt(pin), longPressButtonISR, CHANGE);
}

/**
 * @brief Desabilita interrupção por mudança de estado.
 *
 * @param pin Pino do botão.
 *
 * @details
 * Utilizada para detectar início e fim do pressionamento,
 * permitindo calcular sua duração.
 */
void disableButtonInterruptChange(uint8_t pin) {
  detachInterrupt(digitalPinToInterrupt(pin));
}

/**
 * @brief Verifica se o botão foi pressionado.
 *
 * Se a flag de clique estiver ativa, ela é limpa e retorna `true`.
 *
 * @param buttonPressed Ponteiro para a flag de estado do botão.
 * @return `true` se o botão foi pressionado.
 * @return `false` caso contrário.
 */
bool checkButtonPressed(volatile bool *buttonPressed) {
  if (*buttonPressed) {
    *buttonPressed = false;
    return true;
  }
  return false;
}


/**
 * @brief Retorna o estado atual do modo mute.
 *
 * @return true Sistema está mutado.
 * @return false Sistema não está mutado.
 *
 * @details
 * Alterna o estado interno quando um novo pressionamento é detectado.
 */
bool wasMuted() {
  if (checkButtonPressed(&flagMutePressed)) {
    isMuted = !isMuted;
  }
  return isMuted;
}

/**
 * @brief Verifica ocorrência de long press.
 *
 * @return true Se ocorreu long press.
 * @return false Caso contrário.
 */
bool wasButtonLongPressed() {
  return flagLongPress;
}


/**
 * @brief Verifica ocorrência de short press.
 *
 * @return true Se ocorreu short press.
 * @return false Caso contrário.
 *
 * @note A flag é automaticamente resetada após leitura.
 */
bool wasButtonShortPress(){
  if (flagShortPress == true){
    flagShortPress = false;
    return true;
  }
  return false;
}

/**
 * @brief Reseta o estado do botão mute.
 *
 * @details
 * Define o sistema como não mutado.
 *
 * @note Usado em reinicializações ou mudança de modo.
 */
void resetButtonState() {
  isMuted = false;
}

