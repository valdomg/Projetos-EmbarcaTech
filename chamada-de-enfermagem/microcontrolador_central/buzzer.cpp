/**
 * @file buzzer.cpp
 * @brief Implementação do módulo de controle do buzzer.
 *
 * @details
 * Este módulo é responsável por controlar o buzzer utilizado
 * para emitir alertas sonoros no sistema.
 *
 * As funcionalidades incluem:
 *
 * - Inicialização do hardware do buzzer
 * - Geração de alertas sonoros intermitentes
 * - Controle do número máximo de alertas emitidos
 * - Gerenciamento do estado de notificação sonora
 *
 * O buzzer alterna seu estado (ligado/desligado) em intervalos
 * definidos pela constante `BUZZER_INTERVAL_MS`, gerando um
 * padrão de som intermitente.
 *
 * O número máximo de ciclos de alerta é controlado pela constante
 * `NUMBER_SOUND_ALERTS`.
 *
 * O controle de tempo é realizado utilizando a função `millis()`,
 * evitando o uso de delays bloqueantes e permitindo que o sistema
 * continue executando outras tarefas.
 *
 * @note As definições de pinos e constantes são obtidas de `config.h`.
 *
 * @date 2026
 */

#include "buzzer.h"
#include "log.h"
#include "config.h"

/**
 * @brief Indica se existe uma notificação sonora ativa.
 *
 * Quando `true`, o buzzer deve emitir alertas sonoros
 * até atingir o número máximo definido de ciclos.
 */
bool isBuzzerNotificationEnabled = false;

/**
 * @brief Contador de alertas sonoros já emitidos.
 *
 * Cada vez que o buzzer é ativado (estado HIGH),
 * o contador é incrementado. Quando o número máximo
 * definido em `NUMBER_SOUND_ALERTS` é atingido,
 * o alerta sonoro é automaticamente desativado.
 */
uint8_t numberAlertIssued = 0;

/**
 * @brief Armazena o instante da última alternância do buzzer.
 *
 * @details
 * Utilizada para controlar o intervalo entre mudanças
 * de estado do buzzer, garantindo que ele seja acionado
 * em uma frequência adequada.
 *
 * O controle é baseado na função `millis()`, permitindo
 * operação não bloqueante do sistema.
 */
unsigned long lastBuzzerPlayTime = 0;

/**
 * @brief Inicializa o buzzer.
 *
 * @details
 * Configura o pino do buzzer como saída digital e
 * garante que o dispositivo inicie desligado.
 *
 * @note Deve ser chamada durante o `setup()` do sistema.
 */
void buzzerInit() {
  // Configura o pino do buzzer como saída
  // e garante que inicie desligado
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}


/**
 * @brief Alterna o estado do buzzer para gerar um alerta sonoro.
 *
 * @details
 * Esta função controla o funcionamento do buzzer através
 * de alternâncias periódicas entre os estados HIGH e LOW.
 *
 * O intervalo entre alternâncias é definido pela constante
 * `BUZZER_INTERVAL_MS`.
 *
 * Funcionamento:
 *
 * 1. Verifica se o intervalo de tempo mínimo foi atingido.
 * 2. Alterna o estado do buzzer.
 * 3. Incrementa o contador de alertas quando o buzzer é ativado.
 * 4. Desativa automaticamente o alerta quando o número máximo
 *    de ciclos definido em `NUMBER_SOUND_ALERTS` é atingido.
 *
 * @note Deve ser chamada periodicamente no `loop()` principal.
 */
void toggleBuzzer() {

  unsigned long now = millis();


  if (now - lastBuzzerPlayTime >= BUZZER_INTERVAL_MS) {
    lastBuzzerPlayTime = now;

    if (numberAlertIssued >= NUMBER_SOUND_ALERTS) {
      isBuzzerNotificationEnabled = false;
      digitalWrite(PIN_BUZZER, LOW);
      return;
    }

    // Inverte o estado atual do pino do buzzer:
    // se estava HIGH passa para LOW, e vice-versa
    bool stateButton = !digitalRead(PIN_BUZZER);


    digitalWrite(PIN_BUZZER, stateButton);

    if (stateButton == HIGH) {
      numberAlertIssued++;
    }

    log(LOG_DEBUG, "Buzzer alternou o estado");
  }
}

/**
 * @brief Verifica se existe uma notificação sonora ativa.
 *
 * @return true  Se o sistema possui um alerta sonoro ativo.
 * @return false Caso contrário.
 */
bool doesHaveNotificationBuzzer() {
  return isBuzzerNotificationEnabled;
}

/**
 * @brief Ativa um novo alerta sonoro no sistema.
 *
 * @details
 * Habilita a emissão de alertas pelo buzzer e
 * reinicia o contador de ciclos de som.
 *
 * Após chamada desta função, o buzzer começará
 * a emitir alertas quando `toggleBuzzer()` for executado.
 */
void enableSoundAlert() {
  isBuzzerNotificationEnabled = true;
  numberAlertIssued = 0;
}
