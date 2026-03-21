#ifndef LISTNURSINGCALL_UTILS_H
#define LISTNURSINGCALL_UTILS_H

/**
 * @file listNursingCall_utils.h
 * @brief Declarações globais para gerenciamento da lista de chamadas de enfermaria.
 *
 * @details
 * Este módulo fornece acesso global à estrutura responsável por armazenar
 * as chamadas de enfermagem recebidas pelo sistema.
 *
 * A lista é implementada por meio de uma **lista duplamente encadeada**
 * definida em `DoublyLinkedList_NursingCall.h`. Essa estrutura permite:
 *
 * - Inserção de novas chamadas
 * - Remoção de chamadas
 * - Navegação entre chamadas (anterior/próxima)
 * - Consulta do número total de chamadas
 *
 * Além da lista principal, o módulo também expõe uma **flag de atualização**
 * utilizada para indicar que a lista sofreu alguma modificação.
 *
 * Essa flag é usada principalmente pelo módulo de interface (ex.: display)
 * para evitar atualizações desnecessárias da tela, atualizando apenas quando
 * ocorre alguma alteração na lista.
 *
 * @note
 * As variáveis declaradas neste arquivo são definidas no arquivo
 * `listNursingCall_utils.cpp`.
 *
 * @see DoublyLinkedList_NursingCall.h
 * @see listNursingCall_utils.cpp
 *
 * @date 2026
 */


// -----------------------------------------------------------------------------
// Dependências
// -----------------------------------------------------------------------------

/**
 * @brief Estrutura de lista duplamente encadeada utilizada para armazenar
 * chamadas de enfermagem.
 */
#include "DoublyLinkedList_NursingCall.h"


// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Instância global da lista de chamadas de enfermaria.
 *
 * @details
 * Esta estrutura armazena todas as chamadas recebidas pelo sistema.
 * Ela pode ser acessada por diferentes módulos do firmware, como:
 *
 * - Processamento de mensagens MQTT
 * - Interface do usuário (display)
 * - Lógica de navegação entre chamadas
 */
extern List_NursingCall listCalls;


/**
 * @brief Flag que indica atualização na lista de chamadas.
 *
 * @details
 * Essa variável é utilizada para sinalizar que ocorreu alguma modificação
 * na lista de chamadas (inserção ou remoção).
 *
 * Quando essa flag é definida como `true`, módulos como o display podem
 * atualizar a interface do usuário para refletir as novas informações.
 *
 * A palavra-chave `volatile` é utilizada para garantir que o compilador
 * não otimize o acesso à variável, permitindo que mudanças realizadas
 * em diferentes partes do firmware sejam sempre detectadas corretamente.
 */
extern volatile bool listUpdated;


#endif