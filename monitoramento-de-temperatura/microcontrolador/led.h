/**
 * @file led.h
 * @brief Interface do módulo de controle do LED indicador.
 *
 * Este arquivo define a interface pública para controle do LED do sistema.
 *
 * O LED pode ser utilizado para:
 *
 * - Indicação de funcionamento normal
 * - Sinalização de estados de alerta
 * - Feedback visual para o usuário
 * - Auxílio em debug
 *
 * O controle é realizado de forma não bloqueante utilizando temporização
 * baseada na função millis().
 *
 * @note A implementação está disponível em led.cpp
 */

#ifndef LED_H
#define LED_H
#include <Arduino.h>



// -----------------------------------------------------------------------------
// Funções públicas do módulo
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o hardware do LED.
 *
 * Configura o pino do LED como saída digital e garante que ele inicie desligado.
 *
 * Esta função deve ser chamada uma única vez durante a inicialização do sistema,
 * normalmente dentro da função setup().
 *
 * @code
 * void setup()
 * {
 *     ledInit();
 * }
 * @endcode
 *
 * @note Evita estados indefinidos no momento da energização.
 */
void ledInit();


/**
 * @brief Alterna o estado do LED em intervalos de tempo definidos.
 *
 * Implementa um mecanismo de temporização não bloqueante utilizando millis().
 * O LED alterna entre ligado e desligado automaticamente após o intervalo
 * definido internamente no módulo.
 *
 * Esta abordagem permite que o sistema continue executando outras tarefas
 * simultaneamente.
 *
 * @param now Tempo atual do sistema em milissegundos.
 *
 * Deve ser obtido através da função:
 *
 * @code
 * millis()
 * @endcode
 *
 * @example
 * @code
 * toggleLed(millis());
 * @endcode
 *
 * @note Não utiliza delay(), evitando bloqueio do sistema.
 */
void toggleLed(unsigned long now);


/**
 * @brief Liga o LED.
 *
 * Força o LED para o estado ligado (nível lógico LOW).
 *
 * Esta função pode ser usada quando é necessário manter o LED aceso continuamente.
 *
 * @example
 * @code
 * turnOnLed();
 * @endcode
 */
void turnOnLed();


/**
 * @brief Desliga o LED.
 *
 * Força o LED para o estado desligado (nível lógico HIGH).
 *
 * Pode ser usado para:
 *
 * - Finalizar sinalizações
 * - Economizar energia
 * - Restaurar estado padrão
 *
 * @example
 * @code
 * turnOffLed();
 * @endcode
 */
void turnOffLed();


#endif