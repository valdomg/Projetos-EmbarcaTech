#ifndef DOUBLY_LINKED_LIST_NURSING_CALL_H
#define DOUBLY_LINKED_LIST_NURSING_CALL_H

/**
 * @file doubly_linked_list_nursing_call.h
 * @brief Implementação de lista duplamente encadeada para gerenciamento de chamadas de enfermaria.
 *
 * @details
 * Este módulo implementa uma estrutura de dados do tipo **lista duplamente encadeada**
 * para armazenar e gerenciar chamadas de enfermaria em um sistema embarcado.
 *
 * Cada elemento da lista representa uma chamada ativa de uma enfermaria
 * contendo:
 * - Identificador textual da enfermaria
 * - Identificador único da chamada
 *
 * A estrutura permite:
 * - Inserção de novas chamadas
 * - Navegação entre chamadas (anterior/próxima)
 * - Remoção de chamadas
 * - Remoção automática da chamada mais antiga quando a lista atinge seu limite
 *
 * A lista possui um limite máximo de elementos definido por `MAX_CALLS`
 * para evitar uso excessivo de memória no microcontrolador.
 *
 * @note Estrutura otimizada para sistemas embarcados com memória limitada.
 */


// -----------------------------------------------------------------------------
// Estrutura de nó da lista
// -----------------------------------------------------------------------------

/**
 * @struct NursingCall
 * @brief Estrutura que representa um nó da lista de chamadas.
 *
 * @details
 * Cada nó armazena informações referentes a uma chamada de enfermaria
 * e mantém ponteiros para os nós anterior e posterior, formando
 * uma lista duplamente encadeada.
 */
struct NursingCall {

  /** Identificador textual da enfermaria */
  char infirmary[32];

  /** Identificador único da chamada */
  char id[32];

  /** Ponteiro para o próximo nó da lista */
  NursingCall* next;

  /** Ponteiro para o nó anterior da lista */
  NursingCall* prev;
};


// -----------------------------------------------------------------------------
// Classe de gerenciamento da lista
// -----------------------------------------------------------------------------

/**
 * @class List_NursingCall
 * @brief Classe responsável por gerenciar a lista de chamadas de enfermaria.
 *
 * @details
 * Esta classe implementa uma **lista duplamente encadeada** que permite
 * armazenar, navegar e manipular chamadas de enfermaria.
 *
 * Características principais:
 * - Navegação sequencial entre chamadas
 * - Remoção por ID ou pela posição atual
 * - Controle de limite máximo de chamadas
 * - Remoção automática da chamada mais antiga quando a lista está cheia
 * - Proteção opcional contra remoção do nó atualmente selecionado
 */
class List_NursingCall {

  private:

    /** Ponteiro para o primeiro nó da lista */
    NursingCall* head;

    /** Ponteiro para o último nó da lista */
    NursingCall* tail;

    /**
     * @brief Nó atualmente selecionado.
     *
     * Utilizado para navegação e operações de remoção.
     */
    NursingCall* current;

    /** Número total de nós presentes na lista */
    int total;

    /**
     * @brief Limite máximo de chamadas armazenadas na lista.
     *
     * Este limite evita crescimento indefinido da estrutura
     * em sistemas embarcados com memória restrita.
     */
    static const int MAX_CALLS = 300;

    /**
     * @brief Flag que controla se o nó atual pode ser removido automaticamente.
     *
     * Quando a lista atinge o limite máximo de chamadas,
     * o sistema pode remover a chamada mais antiga para
     * permitir novas inserções.
     *
     * Caso esta flag esteja ativada, o nó `current` não
     * será removido automaticamente.
     */
    bool doNotRemoveCurrent = false;


    /**
     * @brief Verifica se o identificador da enfermaria é válido.
     *
     * @param infirmary Identificador da enfermaria.
     * @return true se a enfermaria for válida.
     * @return false caso contrário.
     */
    bool isValidInfirmary(const char* infirmary);


    /**
     * @brief Remove a chamada mais antiga da lista.
     *
     * @details
     * Utilizado quando a lista atinge o limite máximo (`MAX_CALLS`).
     * Remove o primeiro elemento da lista para liberar espaço.
     *
     * @return true se a remoção foi realizada com sucesso.
     * @return false caso não seja possível remover.
     */
    bool removeOldestCall();


    /**
     * @brief Remove um nó específico da lista.
     *
     * @param node Ponteiro para o nó a ser removido.
     * @return true se a remoção foi realizada com sucesso.
     * @return false caso contrário.
     */
    bool remove(NursingCall* node);


  public:

    /**
     * @brief Construtor da lista.
     *
     * Inicializa a estrutura com a lista vazia.
     */
    List_NursingCall();


    /**
     * @brief Destrutor da lista.
     *
     * Libera toda a memória utilizada pelos nós da lista.
     */
    ~List_NursingCall();


    // -------------------------------------------------------------------------
    // Consultas
    // -------------------------------------------------------------------------

    /**
     * @brief Retorna o número total de chamadas armazenadas.
     *
     * @return Quantidade total de nós na lista.
     */
    int getTotal();


    /**
     * @brief Retorna a enfermaria do nó atualmente selecionado.
     *
     * @return Ponteiro para string com o identificador da enfermaria
     * ou nullptr caso não exista nó atual.
     */
    const char* getInfirmaryCurrent();


    /**
     * @brief Retorna o identificador da chamada atual.
     *
     * @return Ponteiro para string contendo o ID da chamada
     * ou nullptr caso não exista nó atual.
     */
    const char* getIdCurrent();


    /**
     * @brief Verifica se existem chamadas armazenadas na lista.
     *
     * @return true se existir pelo menos uma chamada.
     * @return false se a lista estiver vazia.
     */
    bool hasNursingCall();


    /**
     * @brief Define o comportamento de proteção do nó atual.
     *
     * @param value
     * - true → impede remoção automática do nó `current`
     * - false → permite remoção automática
     */
    void setDoNotRemoveCurrent(bool value);


    /**
     * @brief Retorna o estado atual da proteção do nó `current`.
     *
     * @return true se o nó atual estiver protegido contra remoção automática.
     */
    bool getDoNotRemoveCurrent() const;


    // -------------------------------------------------------------------------
    // Navegação
    // -------------------------------------------------------------------------

    /**
     * @brief Avança para o próximo nó da lista.
     *
     * Atualiza o ponteiro `current` para o próximo elemento.
     */
    void next();


    /**
     * @brief Retrocede para o nó anterior da lista.
     *
     * Atualiza o ponteiro `current` para o elemento anterior.
     */
    void prev();


    // -------------------------------------------------------------------------
    // Modificações
    // -------------------------------------------------------------------------

    /**
     * @brief Adiciona uma nova chamada na lista.
     *
     * @param infirmary Identificador da enfermaria.
     * @param id Identificador único da chamada.
     *
     * @return true se a inserção foi realizada com sucesso.
     * @return false caso contrário.
     */
    bool add(const char* infirmary, const char* id);


    /**
     * @brief Remove o nó atualmente selecionado.
     *
     * @return true se a remoção foi realizada com sucesso.
     * @return false caso não exista nó atual.
     */
    bool removeCurrent();


    /**
     * @brief Remove uma chamada pelo seu identificador.
     *
     * @param id Identificador da chamada.
     * @return true se a chamada foi encontrada e removida.
     * @return false caso contrário.
     */
    bool removalById(const char* id);


    /**
     * @brief Remove todos os elementos da lista.
     *
     * Libera toda a memória ocupada pelos nós.
     */
    void clear();
};

#endif  // DOUBLY_LINKED_LIST_NURSING_CALL_H