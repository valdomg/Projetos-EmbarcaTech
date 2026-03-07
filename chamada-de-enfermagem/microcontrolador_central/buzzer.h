/**
 * @file buzzer.h
 * @brief Interface do módulo de controle do buzzer.
 *
 * @details
 * Este módulo define a interface responsável pelo controle
 * do buzzer utilizado no sistema para emissão de alertas sonoros.
 *
 * O buzzer é acionado quando o sistema detecta uma condição
 * que exige notificação ao usuário, como alarmes ou eventos
 * importantes.
 *
 * Funcionalidades disponíveis:
 *
 * - Inicialização do hardware do buzzer
 * - Ativação de alertas sonoros
 * - Alternância periódica do estado do buzzer
 * - Verificação do estado atual de notificação sonora
 *
 * O controle de tempo é baseado na função `millis()`,
 * permitindo operação não bloqueante dentro do loop principal
 * do firmware.
 *
 * @note As definições de pinos do buzzer são obtidas em `config.h`.
 *
 * @date 2026
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>



/**
 * @brief Intervalo de tempo entre alternâncias do buzzer.
 *
 * @details
 * Define o tempo, em milissegundos, entre cada mudança de estado
 * do buzzer (HIGH → LOW ou LOW → HIGH).
 *
 * Esse valor controla a frequência do alerta sonoro gerado.
 *
 * Valor atual: **1000 ms (1 segundo)**.
 */
constexpr unsigned long BUZZER_INTERVAL_MS = 1000;


/**
 * @brief Inicializa o hardware do buzzer.
 *
 * @details
 * Configura o pino do buzzer como saída digital e garante
 * que o dispositivo inicie desligado.
 *
 * Esta função deve ser chamada no `setup()` antes de qualquer
 * tentativa de uso do buzzer.
 */
void buzzerInit();


/**
 * @brief Controla a alternância do buzzer durante um alerta.
 *
 * @details
 * Alterna periodicamente o estado do buzzer para gerar
 * um padrão sonoro intermitente.
 *
 * A alternância ocorre apenas quando o intervalo definido
 * em `BUZZER_INTERVAL_MS` é atingido.
 *
 * @note Deve ser chamada periodicamente dentro do `loop()` principal.
 */
void toggleBuzzer();


/**
 * @brief Ativa um novo alerta sonoro.
 *
 * @details
 * Habilita a emissão de alertas pelo buzzer. Após chamada
 * desta função, o buzzer começará a alternar seu estado
 * quando `toggleBuzzer()` for executada.
 */
void enableSoundAlert();


/**
 * @brief Verifica se existe uma notificação sonora ativa.
 *
 * @return true  Se o buzzer possui um alerta ativo.
 * @return false Caso contrário.
 */
bool doesHaveNotificationBuzzer();


#endif