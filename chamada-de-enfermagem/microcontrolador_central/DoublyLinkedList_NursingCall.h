#ifndef DOUBLY_LINKED_LIST_NURSING_CALL_H
#define DOUBLY_LINKED_LIST_NURSING_CALL_H


// NursingCall: nó 
struct NursingCall {
    int infirmary;     // valor armazenado
    NursingCall* next; // ponteiro para próximo nó
    NursingCall* prev; // ponteiro para nó anterior
};


#endif // DOUBLY_LINKED_LIST_NURSING_CALL_H