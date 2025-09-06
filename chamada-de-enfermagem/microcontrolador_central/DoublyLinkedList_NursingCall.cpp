#include "DoublyLinkedList_NursingCall.h"  // Inclui o header da lista


/* Construtor:
    Inicializa lista vazia (NULL) */
List_NursingCall::List_NursingCall() {
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    total = 0;
}


/* Destrutor */
List_NursingCall::~List_NursingCall() {
     
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

// retorna true quando há pelo menos um nó
bool List_NursingCall::hasNursingCall() {
    return total > 0;
}

// =========================
//        Add Nó
// =========================
void List_NursingCall::add(int infirmary) { // Adicionar um nó ao final da lista
    NursingCall* new_node = new NursingCall; // Cria um novo nó
    new_node->infirmary = infirmary; // O campo infirmary do novo nó recebe o valor correspondente
    new_node->next = nullptr; // Como vai ser o último nó, o campo next é vazio
    new_node->prev = tail; // Liga o prev do novo ao tail atual

    if (tail != nullptr) { // Se havia um tail, ajusta tail->next
        tail->next = new_node;
    } else { // senão (lista vazia) ajusta head
        head = new_node;
    }

    // Atualiza tail com o novo nó
    tail = new_node;
    // define current como o novo nó
    current = new_node;
    // Incrementa total
    total++;
}