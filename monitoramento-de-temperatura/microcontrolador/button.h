/**
 * @file button.h
 * @brief Interface do módulo de controle de botões com suporte a interrupções.
 *
 * Este módulo fornece uma abstração para o gerenciamento de botões físicos
 * utilizando interrupções de hardware, permitindo a detecção eficiente dos
 * seguintes eventos:
 *
 * - Clique curto (toggle) — utilizado para alternar o estado de mute
 * - Pressão longa (long press e short press) — utilizado para modo configuração e mostrar dados no display lcd
 *
 * O módulo implementa internamente:
 *
 * - Tratamento de debounce
 * - Controle de flags de eventos
 * - Lógica de alternância de estado (toggle)
 *
 * A implementação está localizada em button.cpp.
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>


// ============================================================================
// Inicialização
// ============================================================================

/**
 * @brief Inicializa o pino do botão.
 *
 * Configura o pino especificado como entrada digital com resistor
 * de pull-up interno ativado.
 *
 * Essa configuração é recomendada para botões conectados entre o pino
 * e o GND, evitando estados flutuantes.
 *
 * Esta função deve ser chamada durante a inicialização do sistema
 * (ex.: setup()) antes de habilitar interrupções.
 *
 * @param pin Número do pino digital onde o botão está conectado.
 */
void buttonInit(uint8_t pin);


// ============================================================================
// Controle de interrupções — clique curto
// ============================================================================

/**
 * @brief Habilita a interrupção para detecção de clique curto (mute).
 *
 * Configura uma interrupção para o evento de borda de subida (RISING),
 * indicando que o botão foi pressionado.
 *
 * Quando o evento ocorre, a rotina de interrupção associada é executada,
 * atualizando o estado interno do módulo.
 *
 * @param pin Número do pino do botão.
 */
void enableButtonInterruptRising(uint8_t pin);


/**
 * @brief Desabilita a interrupção do botão de mute.
 *
 * Após esta chamada, eventos de clique curto não serão mais detectados
 * até que a interrupção seja habilitada novamente.
 *
 * @param pin Número do pino do botão.
 */
void disableButtonInterruptRising(uint8_t pin);

// ============================================================================
// Controle de interrupções — pressão longa e pressão curta
// ============================================================================

/**
 * @brief Habilita a interrupção para detecção de pressão longa e curta (botao configuração/turn on).
 *
 * Configura uma interrupção para o evento de mudança de estado (CHANGE),
 * permitindo medir o tempo em que o botão permanece pressionado.
 *
 * Essa interrupção é utilizada internamente para detectar pressões longas.
 *
 * @param pin Número do pino do botão.
 */
void enableButtonInterruptChange(uint8_t pin);

/**
 * @brief Desabilita a interrupção do botão de pressão longa e curta.
 *
 * Após esta chamada, eventos de pressão longa não serão mais detectados.
 *
 * @param pin Número do pino do botão.
 */
void disableButtonInterruptChange(uint8_t pin);

// ============================================================================
// Consulta de estado
// ============================================================================

/**
 * @brief Verifica se o sistema entrou no estado de mute.
 *
 * Essa função verifica se ocorreu um evento de clique curto e alterna
 * o estado interno de mute.
 *
 * @return true  Sistema está em mute.
 * @return false Sistema está ativo.
 */
bool wasMuted();

/**
 * @brief Verifica se ocorreu um evento de pressão longa.
 *
 * Retorna true apenas uma vez para cada evento detectado.
 *
 * Após a leitura, o estado interno é mantido ou redefinido conforme
 * a implementação no arquivo button.cpp.
 *
 * @return true  Pressão longa detectada.
 * @return false Nenhuma pressão longa detectada.
 */
bool wasButtonLongPressed();

/**
 * @brief Verifica se ocorreu um evento de clique curto no botao de configuracao/ligar lcd.
 * 
 *
 * @return true  Clique curto detectado.
 * @return false Nenhum evento detectado.
 */
bool wasButtonShortPress();


// ============================================================================
// Controle de estado
// ============================================================================

/**
 * @brief Reseta completamente o estado interno do botão.
 *
 * Define o sistema como não mutado e limpa flags internas associadas.
 *
 * Deve ser utilizada em:
 *
 * - Inicialização do sistema
 * - Reinicializações
 * - Reset manual do estado lógico
 */
void resetButtonState();


#endif
