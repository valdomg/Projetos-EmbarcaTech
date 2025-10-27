#include "DoublyLinkedList_NursingCall.h"  // Inclui o header da lista
#include <cstring> // para strncpy


/* Construtor:
    Inicializa lista vazia (NULL) */
List_NursingCall::List_NursingCall() {
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    total = 0;
}


/* Destrutor:
    - chama clear() para liberar memória dos nós antes de o objeto ser destruído 
    - previne vazamentos se a lista ainda tiver nós */
List_NursingCall::~List_NursingCall() {
    clear(); 
}


// =========================
//        Consultas
// =========================
int List_NursingCall::getTotal() { // retorna o número de nós
    return total;
}

// retorna o campo infirmary do nó current ou -1 se não houver
int List_NursingCall::getInfirmaryCurrent() {
    if (current != nullptr) return current->infirmary;
    return -1;
}

// retorna o campo ID do nó current
const char* List_NursingCall::getIdCurrent() {
    if (current) return current->id;
    return nullptr;
}

// retorna true quando há pelo menos um nó
bool List_NursingCall::hasNursingCall() {
    return total > 0;
}

// =========================
//        Add Nó
// =========================
void List_NursingCall::add(int infirmary, const char* id) { // Adicionar um nó ao final da lista
    NursingCall* new_node = new NursingCall; // Cria um novo nó
    new_node->infirmary = infirmary; // O campo infirmary do novo nó recebe o valor correspondente

    // Copia o ID 
    strncpy(new_node->id, id, sizeof(new_node->id) - 1);
    new_node->id[sizeof(new_node->id) - 1] = '\0';

    new_node->next = nullptr; // Como vai ser o último nó, o campo next é vazio
    new_node->prev = tail; // Liga o prev do novo ao tail atual

    if (tail != nullptr) { // Se havia um tail, ajusta tail->next
        tail->next = new_node;
    } else { // senão (lista vazia) ajusta head
        head = new_node;
        // define current como o novo nó
        current = new_node;
    }

    // Atualiza tail com o novo nó
    tail = new_node;
    // Incrementa total
    total++;
}


// =========================
//        Navegações
// =========================
void List_NursingCall::next() { // Move current para próximo nó
    // Se o current ou o próximo nó de current não for vazio 
    if (current != nullptr && current->next != nullptr) {
        current = current->next;
    }
}

void List_NursingCall::prev() { // Move current para o nó anterior
    // Se o current ou o nó anterior de current não for vazio 
    if (current != nullptr && current->prev != nullptr) {
        current = current->prev;
    }
}


// =========================
//        Remoção
// =========================
void List_NursingCall::removeCurrent() {
    // Se current é NULL, não remove.
    if (current == nullptr) return; 

    // variável auxiliar para segurar o nó que será removido 
    NursingCall* temp = current;

    /* ==============================
        Ajustar o ponteiro anterior
       ==============================*/
    // Se existe um nó anterior, faz esse nó apontar (next) para o próximo de temp
    if (temp->prev != nullptr) {
        temp->prev->next = temp->next;
    } else { // Caso contrário - temp é o primeiro da lista
        // Atualiza head para ser o próximo nó
        head = temp->next;
    }

    /* ==============================
        Ajustar o ponteiro seguinte
       ==============================*/
    if (temp->next != nullptr) {
        // Se existe um nó seguinte - faz esse nó apontar (prev) para o anterior de temp
        temp->next->prev = temp->prev;
        // Atualiza current para esse próximo nó
        current = temp->next;
    } else { // Caso contrário - temp é último da lista
        // Atualiza tail para o anterior de temp
        tail = temp->prev;
        // Atualiza current para esse anterior
        current = temp->prev;
    }

    // Remove da memória o nó que foi desconectado, evitando vazamento de memória
    delete temp;
    // Decrementa o número de nós da lista
    total--;
}

// Limpar toda a lista
void List_NursingCall::clear() {
    // Cria um ponteiro auxiliar - começa apontando para o primeiro nó da lista
    NursingCall* temp = head;
    while (temp != nullptr) { // enquanto ainda houver nós na lista
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