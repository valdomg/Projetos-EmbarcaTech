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