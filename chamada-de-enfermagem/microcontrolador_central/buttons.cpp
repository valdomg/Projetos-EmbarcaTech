/**
 * @file buttons.cpp
 * @brief Implementação do gerenciamento de botões do sistema.
 *
 * @details
 * Este módulo implementa a leitura e o controle de múltiplos botões
 * físicos utilizados para interação com o usuário.
 *
 * Os botões disponíveis no sistema são:
 *
 * - Botão **Next** → navegação para próximo item
 * - Botão **Prev** → navegação para item anterior
 * - Botão **Delete** → remoção de item
 *
 * Funcionalidades implementadas:
 *
 * - Leitura de botões com debounce por software
 * - Detecção de pressionamento (evento de clique)
 * - Detecção de pressionamento longo (long press)
 * - Uso de interrupções para ativação de modo de configuração
 *
 * O debounce é realizado por software utilizando um intervalo mínimo
 * entre leituras válidas, evitando múltiplas detecções causadas pelo
 * ruído mecânico dos botões.
 *
 * O botão **Prev** também é utilizado para detectar um pressionamento
 * longo (15 segundos), que alterna o modo de configuração do sistema.
 *
 * @note
 * Este módulo depende da estrutura `Button` definida em `buttons.h`.
 *
 * @warning
 * Variáveis utilizadas em rotinas de interrupção são declaradas como
 * `volatile` para garantir comportamento correto em ambientes concorrentes.
 *
 * @date 2026
 */

#include "buttons.h"

/**
 * @brief Tempo mínimo para debounce dos botões.
 *
 * Define o intervalo mínimo (em milissegundos) que o sinal do botão
 * deve permanecer estável antes de ser considerado uma leitura válida.
 *
 * Isso evita múltiplas leituras causadas pelo ruído mecânico
 * do contato físico do botão.
 */
constexpr unsigned long DEBOUNCE_DELAY = 50;


/**
 * @brief Armazena o instante em que o botão foi pressionado.
 *
 * Utilizado para calcular a duração do pressionamento do botão
 * e detectar eventos de long press.
 */
unsigned long buttonPressStartTime = 0;

/**
 * @brief Indica se o botão está atualmente pressionado.
 *
 * Esta variável é utilizada durante a rotina de interrupção
 * para detectar a transição entre pressionado e liberado.
 */
volatile bool isButtonPressed = false;

/**
 * @brief Flag que indica se o modo de configuração foi ativado.
 *
 * Esta variável alterna seu estado quando ocorre um
 * pressionamento longo do botão configurado.
 */
volatile bool isButtonLongPressActivate = false;

/**
 * @brief Instância do botão "Next".
 *
 * Utilizado para navegar para o próximo item ou registro.
 */
Button button_next = { 14, HIGH, HIGH, 0 };

/**
 * @brief Instância do botão "Previous".
 *
 * Utilizado para navegar para o item anterior.
 * Também é responsável por detectar o pressionamento longo
 * que ativa o modo de configuração do sistema.
 */
Button button_prev = { 12, HIGH, HIGH, 0 };

/**
 * @brief Instância do botão "Delete".
 *
 * Utilizado para remover registros ou itens selecionados.
 */
Button button_delete = { 13, HIGH, HIGH, 0 };


/**
 * @brief Rotina de interrupção para detecção de pressionamento longo.
 *
 * @details
 * Esta função é executada automaticamente quando ocorre uma
 * mudança de estado (CHANGE) no pino do botão `button_prev`.
 *
 * Funcionamento:
 *
 * - Quando o botão é pressionado (LOW), registra o tempo atual.
 * - Quando o botão é liberado (HIGH), calcula o tempo pressionado.
 * - Se o tempo for maior ou igual a **15 segundos**, o modo de
 *   configuração do sistema é alternado.
 *
 * @note
 * Esta função é executada em contexto de interrupção e deve
 * permanecer o mais curta possível.
 */

void IRAM_ATTR longPressButtonISR() {

  bool readButton = digitalRead(button_prev.pin);

  if (readButton == LOW) {
    log(LOG_INFO,"botao pressionado");
    buttonPressStartTime = millis();
    isButtonPressed = true;
  } else if (readButton == HIGH) {

    if (millis() - buttonPressStartTime >= 15 * 1000){
      isButtonLongPressActivate = !isButtonLongPressActivate;
    }
    log(LOG_INFO,"botao solto");

    isButtonPressed = false;
  }
}

/**
 * @brief Inicializa os botões do sistema.
 *
 * Configura os pinos dos botões como entrada com resistor
 * pull-up interno ativado.
 *
 * Com essa configuração:
 * - HIGH → botão não pressionado
 * - LOW  → botão pressionado
 */
void initButtons() {
  // Configura os pinos como entrada com pull-up interno (LOW quando pressionado)
  pinMode(button_next.pin, INPUT_PULLUP);
  pinMode(button_prev.pin, INPUT_PULLUP);
  pinMode(button_delete.pin, INPUT_PULLUP);
}

/**
 * @brief Verifica se um botão foi pressionado.
 *
 * @param btn Referência para a estrutura do botão.
 *
 * @return true  Se um pressionamento válido foi detectado.
 * @return false Caso contrário.
 *
 * @details
 * Implementa debounce por software verificando se o estado
 * do botão permaneceu estável por um período mínimo definido
 * em `DEBOUNCE_DELAY`.
 *
 * A função retorna `true` apenas no momento em que o botão
 * é pressionado (transição HIGH → LOW).
 */
bool checkButton(Button &btn) {
  uint8_t reading = digitalRead(btn.pin);

  if (reading != btn.lastRawReading) {  // Reinicia o timer se a leitura muda
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

/**
 * @brief Habilita a interrupção para detecção de long press.
 *
 * Associa o pino do botão `button_prev` à rotina de interrupção
 * responsável por detectar pressionamentos longos.
 */
void enableButtonInterruptChange(){
  attachInterrupt(digitalPinToInterrupt(button_prev.pin), longPressButtonISR, CHANGE);
}


/**
 * @brief Verifica se o sistema está em modo de configuração.
 *
 * @return true  Se o modo de configuração está ativo.
 * @return false Caso contrário.
 *
 * @details
 * O modo de configuração é ativado ou desativado através de
 * um pressionamento longo do botão `button_prev`.
 */
bool isConfigurationMode(){
  return isButtonLongPressActivate;
}