#include "listNursingCall_utils.h"


//      Variaveis globais para uso da lista
// Lista de chamadas de enfermagem 
List_NursingCall listCalls;
// flag que indica que houve atualização na lista para indicar que o display precisa ser atualizado
volatile bool listUpdated = false;