/**
 * @file buttons.h
 * @brief Interface do módulo de gerenciamento de botões físicos.
 *
 * @details
 * Este módulo define a estrutura e as funções responsáveis
 * pela leitura e gerenciamento de botões físicos no sistema.
 *
 * Os botões são utilizados para interação com o usuário,
 * permitindo navegação entre registros e execução de ações
 * como exclusão ou entrada no modo de configuração.
 *
 * Funcionalidades fornecidas:
 *
 * - Estrutura de dados para controle de botões
 * - Leitura com debounce por software
 * - Inicialização dos pinos de entrada
 * - Detecção de eventos de pressionamento
 * - Detecção de pressionamento longo via interrupção
 *
 * O módulo trabalha com três botões principais:
 *
 * - **button_next** → navegação para o próximo item
 * - **button_prev** → navegação para o item anterior
 * - **button_delete** → exclusão de registros
 *
 * O botão `button_prev` também é utilizado para ativar
 * o modo de configuração do sistema através de um
 * pressionamento longo.
 *
 * @note
 * A implementação das funções está no arquivo `buttons.cpp`.
 *
 * @date 2026
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include "log.h"
#include <Arduino.h>

/**
 * @struct Button
 * @brief Estrutura que representa um botão físico do sistema.
 *
 * @details
 * Armazena informações necessárias para realizar a leitura
 * do botão com tratamento de debounce por software.
 *
 * Cada botão mantém o controle do estado atual, da última
 * leitura bruta e do instante da última mudança detectada.
 */
struct Button {

  /**
   * @brief Pino do microcontrolador onde o botão está conectado.
   */
  uint8_t pin;

  /**
   * @brief Estado atual do botão após aplicação de debounce.
   *
   * - LOW  → botão pressionado
   * - HIGH → botão não pressionado
   */
  uint8_t state;

  /**
   * @brief Última leitura bruta do pino (sem debounce).
   *
   * Utilizada para detectar mudanças de estado do botão.
   */
  uint8_t lastRawReading;

  /**
   * @brief Instante da última mudança detectada na leitura bruta.
   *
   * Utilizado para implementar o debounce por software,
   * garantindo que a leitura seja considerada válida
   * somente após um intervalo mínimo de estabilidade.
   */
  unsigned long lastDebounceTime;
};


/**
 * @brief Instância do botão "Next".
 *
 * Utilizado para navegar para o próximo item ou registro.
 */
extern Button button_next;

/**
 * @brief Instância do botão "Previous".
 *
 * Utilizado para navegar para o item anterior.
 * Também é responsável por detectar pressionamento longo
 * para ativação do modo de configuração.
 */
extern Button button_prev;

/**
 * @brief Instância do botão "Delete".
 *
 * Utilizado para remover registros ou itens selecionados.
 */
extern Button button_delete;

/**
 * @brief Inicializa os botões do sistema.
 *
 * Configura os pinos definidos nas estruturas `Button`
 * como entradas com resistor pull-up interno ativado.
 *
 * @note Deve ser chamada durante o `setup()` do sistema.
 */
extern void initButtons();


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
 * do botão permaneceu estável por um período mínimo antes
 * de confirmar o evento de pressionamento.
 *
 * A função retorna `true` apenas quando detecta uma transição
 * válida para o estado pressionado.
 */
extern bool checkButton(Button &btn);


/**
 * @brief Habilita interrupção para detecção de pressionamento longo.
 *
 * Associa o botão responsável pelo modo de configuração
 * à rotina de interrupção que monitora mudanças de estado
 * do pino e calcula a duração do pressionamento.
 */
void enableButtonInterruptChange();


/**
 * @brief Verifica se o sistema está em modo de configuração.
 *
 * @return true  Se o modo de configuração estiver ativo.
 * @return false Caso contrário.
 *
 * @details
 * O modo de configuração é ativado através de um
 * pressionamento longo do botão `button_prev`.
 */
bool isConfigurationMode();

#endif