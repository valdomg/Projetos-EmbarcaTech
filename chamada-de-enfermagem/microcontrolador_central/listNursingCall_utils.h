#ifndef LISTNURSINGCALL_UTILS_H
#define LISTNURSINGCALL_UTILS_H


// implementa a lista duplamente ligada para armazenar as chamadas de enfermagem
#include "DoublyLinkedList_NursingCall.h"


// Lista de chamadas de enfermagem 
extern List_NursingCall listCalls;
// flag que indica que houve atualização na lista para indicar que o display precisa ser atualizado
extern volatile bool listUpdated;


#endif