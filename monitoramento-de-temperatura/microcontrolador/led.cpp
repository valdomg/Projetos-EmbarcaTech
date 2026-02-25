/**
 * @file led.cpp
 * @brief Implementação das funções de controle do LED indicador do sistema.
 *
 * Este arquivo contém a implementação das funções responsáveis por inicializar,
 * ligar, desligar e alternar o estado do LED.
 *
 * O controle é feito de forma não bloqueante utilizando a função millis(),
 * permitindo que o LED pisque sem interromper a execução das demais tarefas
 * do sistema.
 *
 * O LED pode ser usado para:
 * - Indicação de funcionamento
 * - Sinalização de alerta
 * - Debug visual do sistema
 */

#include "led.h"
#include "config.h"


// -----------------------------------------------------------------------------
// Variáveis globais privadas ao módulo
// -----------------------------------------------------------------------------

/**
 * @brief Armazena o instante da última alternância do LED.
 *
 * Esta variável é usada como referência para calcular o tempo decorrido
 * desde a última mudança de estado do LED.
 *
 * @note Utilizada para controle não bloqueante baseado em millis().
 */
unsigned long lastTimeLedTurnedOn = 0;


/**
 * @brief Intervalo de tempo para alternância do LED.
 *
 * Define o período de piscar do LED em milissegundos.
 *
 * Valor atual:
 * - 2000 ms = 2 segundos
 *
 * @note constexpr garante valor constante em tempo de compilação.
 */
constexpr unsigned long LED_INTERVAL_MS = 2000;


// -----------------------------------------------------------------------------
// Implementação das funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o hardware do LED.
 *
 * Configura o pino definido em PIN_LED como saída digital
 * e garante que o LED inicie desligado.
 *
 * Esta função deve ser chamada uma única vez durante a inicialização
 * do sistema, normalmente dentro da função setup().
 *
 * @note Evita estados indefinidos no boot.
 */
void ledInit()
{
    pinMode(PIN_LED, OUTPUT);

    // Garante estado inicial desligado
    digitalWrite(PIN_LED, LOW);
}


/**
 * @brief Alterna o estado do LED em intervalos definidos.
 *
 * Esta função implementa um temporizador não bloqueante utilizando millis().
 * O LED muda de estado somente quando o intervalo definido em
 * LED_INTERVAL_MS é atingido.
 *
 * Isso permite que o sistema continue executando outras tarefas
 * simultaneamente.
 *
 * @param now Tempo atual do sistema em milissegundos.
 * Normalmente obtido através da função:
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
 * @note Não utiliza delay(), garantindo melhor desempenho do sistema.
 */
void toggleLed(unsigned long now)
{
    if (now - lastTimeLedTurnedOn >= LED_INTERVAL_MS)
    {
        lastTimeLedTurnedOn = now;

        // Alterna estado atual
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }
}


/**
 * @brief Liga o LED.
 *
 * Força o pino do LED para nível lógico HIGH.
 *
 * @note Usado para sinalização contínua.
 */
void turnOnLed()
{
    digitalWrite(PIN_LED, HIGH);
}


/**
 * @brief Desliga o LED.
 *
 * Força o pino do LED para nível lógico LOW.
 *
 * @note Pode ser usado para economizar energia.
 */
void turnOffLed()
{
    digitalWrite(PIN_LED, LOW);
}