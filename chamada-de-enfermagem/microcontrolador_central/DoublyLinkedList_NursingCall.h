#ifndef DOUBLY_LINKED_LIST_NURSING_CALL_H
#define DOUBLY_LINKED_LIST_NURSING_CALL_H


// NursingCall: nó 
struct NursingCall {
    int infirmary;     // número do quarto valor armazenado
    char id[32];       // id da chamada (tamanho fixo)
    NursingCall* next; // ponteiro para próximo nó
    NursingCall* prev; // ponteiro para nó anterior
};


class List_NursingCall {
  private:
    NursingCall* head;    // primeiro nó
    NursingCall* tail;    // último nó
    NursingCall* current; // nó selecionado (para navegação e remoção)
    int total;            // número de nós


  // área pública que permite manipular e consultar a lista
  public:
    List_NursingCall();
    ~List_NursingCall();

    int getTotal();
    int getInfirmaryCurrent();
    const char* getIdCurrent();
    bool hasNursingCall();

    void add(int infirmary, const char* id);
    void next();
    void prev();
    void removeCurrent();
    void clear(); 
};

#endif // DOUBLY_LINKED_LIST_NURSING_CALL_H