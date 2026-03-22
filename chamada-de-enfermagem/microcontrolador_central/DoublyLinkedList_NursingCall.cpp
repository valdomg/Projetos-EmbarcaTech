
/**
 * @file DoublyLinkedList_NursingCall.cpp
 * @brief Implementação de uma lista duplamente encadeada para gerenciamento de chamadas de enfermagem.
 *
 * @details
 * Este módulo implementa uma estrutura de dados baseada em lista duplamente
 * encadeada para armazenar chamadas de enfermagem pendentes.
 *
 * Cada nó da lista representa um chamado contendo:
 * - Número da enfermaria
 * - Identificador do dispositivo
 *
 * Funcionalidades principais:
 * - Inserção de novos chamados
 * - Navegação entre chamados
 * - Remoção de chamados
 * - Remoção automática quando atingir limite de memória
 * - Busca e remoção por ID
 * - Limpeza completa da lista
 *
 * A lista mantém ponteiros para:
 * - Primeiro elemento (`head`)
 * - Último elemento (`tail`)
 * - Elemento atual (`current`)
 *
 * Isso permite navegação eficiente entre os nós.
 */

#include "DoublyLinkedList_NursingCall.h"  // Inclui o header da lista
#include <cstring>                         // para strncpy
#include "log.h"


/**
 * @brief Construtor da lista.
 *
 * @details
 * Inicializa uma lista vazia definindo todos os ponteiros
 * como `nullptr` e o contador de elementos como zero.
 */
List_NursingCall::List_NursingCall() {
  head = nullptr;
  tail = nullptr;
  current = nullptr;
  total = 0;
}


/**
 * @brief Destrutor da lista.
 *
 * @details
 * Libera toda memória ocupada pelos nós da lista
 * chamando o método `clear()` antes da destruição
 * do objeto.
 *
 * Isso evita vazamentos de memória.
 */
List_NursingCall::~List_NursingCall() {
  clear();
}


// =========================
//        Consultas
// =========================

/**
 * @brief Retorna a quantidade total de nós na lista.
 *
 * @return Número de elementos armazenados na lista.
 */
int List_NursingCall::getTotal() {  // retorna o número de nós
  return total;
}


/**
 * @brief Retorna a enfermaria do nó atual.
 *
 * @return Ponteiro para string contendo o número da enfermaria
 * ou `nullptr` caso não exista elemento atual.
 */
const char* List_NursingCall::getInfirmaryCurrent() {
  if (current != nullptr) return current->infirmary;
  return nullptr;
}

/**
 * @brief Retorna o ID do dispositivo associado ao nó atual.
 *
 * @return Ponteiro para string contendo o ID ou `nullptr`
 * caso não exista elemento atual.
 */
const char* List_NursingCall::getIdCurrent() {
  if (current) return current->id;
  return nullptr;
}

/**
 * @brief Verifica se existem chamados armazenados.
 *
 * @return true se existir pelo menos um chamado.
 * @return false se a lista estiver vazia.
 */
bool List_NursingCall::hasNursingCall() {
  return total > 0;
}

/**
 * @brief Valida o número da enfermaria informado.
 *
 * @param infirmary String contendo o número da enfermaria.
 *
 * @return true se a enfermaria for válida.
 * @return false se for nula ou vazia.
 */
bool List_NursingCall::isValidInfirmary(const char* infirmary) {
  if (infirmary == nullptr || infirmary[0] == '\0') return false;
  return true;
}

/**
 * @brief Define se o nó atual pode ser removido automaticamente.
 *
 * @details
 * Este parâmetro controla a política de remoção automática
 * quando a lista atinge seu limite máximo de elementos.
 *
 * @param value Valor booleano indicando permissão de remoção.
 */
void List_NursingCall::setDoNotRemoveCurrent(bool value) {
  doNotRemoveCurrent = value;
}

/**
 * @brief Retorna a configuração de proteção do nó atual.
 *
 * @return true se o nó atual não pode ser removido automaticamente.
 * @return false caso contrário.
 */
bool List_NursingCall::getDoNotRemoveCurrent() const {
  return doNotRemoveCurrent;
}


// =========================
//        Adição de Nó
// =========================

/**
 * @brief Adiciona um novo chamado ao final da lista.
 *
 * @details
 * Cria um novo nó contendo o número da enfermaria
 * e o ID do dispositivo que gerou o chamado.
 *
 * Caso a lista atinja o limite máximo (`MAX_CALLS`),
 * chamadas mais antigas são removidas automaticamente
 * seguindo a política FIFO.
 *
 * @param infirmary Número da enfermaria.
 * @param id Identificador do dispositivo que gerou o chamado.
 *
 * @return true se o nó foi adicionado com sucesso.
 * @return false se ocorrer erro de validação ou alocação.
 */
bool List_NursingCall::add(const char* infirmary, const char* id) {  // Adicionar um nó ao final da lista
  // verifica se o ID do dispositivo é valido
  if (id == nullptr || id[0] == '\0') return false;
  
  // verifica se o número do quarto é valido
  if (!isValidInfirmary(infirmary)) return false;

  // gerencia o limite de memória
  if (total >= MAX_CALLS && !doNotRemoveCurrent) {  // Verifica se lista atingiu capacidade máxima
    // loop: apaga todos os elementos que atingiram o limite se não estiver na tela de confirmação de exclusão
    while (total >= MAX_CALLS) {
      if (!removeOldestCall()) return false;  // se atingiu o limite, tenta remover chamada mais antiga (política FIFO)
    }
  }

  NursingCall* new_node = new NursingCall;  // Cria um novo nó
  if (new_node == nullptr) return false; // verifica se alocação foi bem-sucedida

  // Copia a enfermaria
  strncpy(new_node->infirmary, infirmary, sizeof(new_node->infirmary) - 1);
  new_node->infirmary[sizeof(new_node->infirmary) - 1] = '\0';
  
  // Copia o ID
  strncpy(new_node->id, id, sizeof(new_node->id) - 1);
  new_node->id[sizeof(new_node->id) - 1] = '\0';

  new_node->next = nullptr;  // Como vai ser o último nó, o campo next é vazio
  new_node->prev = tail;     // Liga o prev do novo ao tail atual

  if (tail != nullptr) {  // Se havia um tail, ajusta tail->next
    tail->next = new_node;
  } else {  // senão (lista vazia) ajusta head
    head = new_node;
    // define current como o novo nó
    current = new_node;
  }

  // Atualiza tail com o novo nó
  tail = new_node;
  // Incrementa total
  total++;
  return true;  // elemento adicionado com sucesso
}


// =========================
//        Navegação
// =========================

/**
 * @brief Move o ponteiro atual para o próximo nó.
 *
 * @details
 * Caso exista um próximo elemento na lista,
 * o ponteiro `current` é atualizado para ele.
 */
void List_NursingCall::next() {  // Move current para próximo nó
  // Se o current ou o próximo nó de current não for vazio
  if (current != nullptr && current->next != nullptr) {
    current = current->next;
  }
}

/**
 * @brief Move o ponteiro atual para o nó anterior.
 *
 * @details
 * Caso exista um nó anterior na lista,
 * o ponteiro `current` é atualizado para ele.
 */
void List_NursingCall::prev() {  // Move current para o nó anterior
  // Se o current ou o nó anterior de current não for vazio
  if (current != nullptr && current->prev != nullptr) {
    current = current->prev;
  }
}


// =========================
//        Remoção
// =========================

/**
 * @brief Remove um nó específico da lista.
 *
 * @param node Ponteiro para o nó a ser removido.
 *
 * @return true se a remoção for bem-sucedida.
 * @return false se a lista estiver vazia ou o nó for inválido.
 */
bool List_NursingCall::remove(NursingCall* node) {

  if (!hasNursingCall()) {
    return false;
  }

  if (node == nullptr) return false;
  /* ==============================
        Ajustar o ponteiro anterior
     ==============================*/
  // Se existe um nó anterior, faz esse nó apontar (next) para o próximo de temp
  if (node->prev != nullptr) {
    node->prev->next = node->next;
  } else {  // Caso contrário - node é o primeiro da lista
    // Atualiza head para ser o próximo nó
    head = node->next;
  }

  /* ==============================
        Ajustar o ponteiro seguinte
       ==============================*/
  if (node->next != nullptr) {
    // Se existe um nó seguinte - faz esse nó apontar (prev) para o anterior de node
    node->next->prev = node->prev;
    // Atualiza current para esse próximo nó

    if (current == node) {
      current = node->next;
    }
  } else {  // Caso contrário - node é último da lista
    // Atualiza tail para o anterior de node
    tail = node->prev;
    // Atualiza current para esse anterior

    if (current == node) {
      current = node->prev;
    }
  }

  // Remove da memória o nó que foi desconectado, evitando vazamento de memória
  delete node;
  // Decrementa o número de nós da lista
  total--;
  return true;
}

/**
 * @brief Remove o nó atual da lista.
 *
 * @return true se a remoção ocorrer com sucesso.
 */
bool List_NursingCall::removeCurrent() {

  // variável auxiliar para segurar o nó que será removido
  NursingCall* temp = current;

  return remove(temp);

}

/**
 * @brief Remove o chamado mais antigo da lista.
 *
 * @details
 * Remove o primeiro elemento da lista seguindo
 * a política FIFO (First In First Out).
 *
 * @return true se o elemento foi removido.
 * @return false se a lista estiver vazia.
 */
bool List_NursingCall::removeOldestCall() {
  if (head == nullptr) return false;  // Verifica se lista está vazia (Se vazia: retorna false (nada para remover))

  // Salva referência ao elemento a ser removido
  NursingCall* toRemove = head;

  // Ajusta head para apontar para o segundo elemento
  head = head->next;  // Se havia apenas 1 elemento: head se torna nullptr

  if (head != nullptr) {  // caso a lista não esteja vazia
    // Remove link anterior do novo primeiro
    head->prev = nullptr;
  } else {
    // Lista ficou vazia atualiza tail para nullptr
    tail = nullptr;
  }

  // Se current apontava para elemento removido
  if (current == toRemove) {
    current = head;  // Redireciona current para o novo head
  }

  delete toRemove;  // Libera memória do elemento removido
  total--;          // Decrementa contador de elementos
  return true;      // Retorna sucesso - elemento removido com sucesso
}

/**
 * @brief Remove um chamado utilizando o ID do dispositivo.
 *
 * @param id Identificador do dispositivo.
 *
 * @return true se o elemento foi encontrado e removido.
 * @return false se nenhum elemento com esse ID existir.
 */
bool List_NursingCall::removalById(const char* id) {

  NursingCall* node = head;

  while (node != nullptr) {
    if (strcmp(node->id, id)==0) {
      //remove no da liista
      remove(node);
      return true;
    }
    node = node->next;
  }
  return false;
}


/**
 * @brief Remove todos os nós da lista.
 *
 * @details
 * Percorre toda a lista liberando a memória
 * de cada nó e reinicializa os ponteiros da lista.
 */
void List_NursingCall::clear() {
  // Cria um ponteiro auxiliar - começa apontando para o primeiro nó da lista
  NursingCall* temp = head;
  while (temp != nullptr) {  // enquanto ainda houver nós na lista
    // Salva o endereço do próximo nó antes de apagar o nó atual
    NursingCall* prox = temp->next;
    // Deletando o nó atual
    delete temp;
    // Faz temp apontar para o próximo nó salvo em prox
    temp = prox;
  }
  // Reseta a lista
  head = nullptr;
  tail = nullptr;
  current = nullptr;
  total = 0;
}