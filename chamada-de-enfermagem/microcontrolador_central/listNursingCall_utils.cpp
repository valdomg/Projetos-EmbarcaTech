#include "listNursingCall_utils.h"

/**
 * @file listNursingCall_utils.cpp
 * @brief Definição das variáveis globais utilizadas no gerenciamento da lista de chamadas de enfermaria.
 *
 * @details
 * Este módulo define instâncias globais utilizadas para o gerenciamento
 * das chamadas de enfermaria no sistema.
 *
 * A lista de chamadas é implementada através da classe `List_NursingCall`,
 * que utiliza uma estrutura de **lista duplamente encadeada** para armazenar
 * e navegar entre as chamadas recebidas.
 *
 * Além da lista principal, também é utilizada uma **flag de atualização**
 * que indica quando houve alteração na lista, permitindo que outros módulos
 * (como o display) saibam quando precisam atualizar as informações exibidas.
 *
 * Esse mecanismo evita atualizações desnecessárias do display, melhorando
 * a eficiência do sistema.
 *
 * @see List_NursingCall
 * @see listNursingCall_utils.h
 *
 * @date 2026
 */


// -----------------------------------------------------------------------------
// Variáveis globais para uso da lista de chamadas
// -----------------------------------------------------------------------------

/**
 * @brief Instância global da lista de chamadas de enfermaria.
 *
 * @details
 * Esta estrutura armazena todas as chamadas recebidas pelo sistema,
 * permitindo inserção, remoção e navegação entre os registros.
 *
 * A lista é manipulada por diferentes módulos do sistema, como:
 * - Processamento de mensagens MQTT
 * - Interface de usuário (display)
 */
List_NursingCall listCalls;


/**
 * @brief Flag que indica atualização na lista de chamadas.
 *
 * @details
 * Esta variável é utilizada para sinalizar que a lista de chamadas
 * foi modificada (inserção ou remoção).
 *
 * Quando essa flag é ativada (`true`), o módulo responsável pela
 * interface do usuário (ex.: display LCD) pode atualizar as
 * informações exibidas.
 *
 * A variável é declarada como `volatile` para garantir que o compilador
 * não realize otimizações indevidas, já que seu valor pode ser alterado
 * por diferentes partes do firmware.
 */
volatile bool listUpdated = false;