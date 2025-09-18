#include "wifi_utils.h"

#include "display_LCD-2004_I2C.h"
// implementa a lista duplamente ligada para armazenar as chamadas de enfermagem
#include "DoublyLinkedList_NursingCall.h"
#include "buttons.h"

// Lista de chamadas de enfermagem 
List_NursingCall listCalls;
// flag que indica se o botão de deletar foi pressionado uma vez e está aguardando confirmação
bool deletionConfirmation = false;


// ===== Funções de navegação =====
void handleNext() { // ===== Botão Next (>)
  if (deletionConfirmation) {
    deletionConfirmation = false; // cancela exclusão ao navegar
  } else {
    listCalls.next();
    Serial.printf("Clicou next %d \n", listCalls.getInfirmaryCurrent());
  }
}

void handlePrev() { // ===== Botão Prev (<)
  if (deletionConfirmation) {
    deletionConfirmation = false; // cancela exclusão ao navegar
  } else {
    listCalls.prev();
    Serial.printf("Clicou prev %d \n", listCalls.getInfirmaryCurrent());
  }
}

void handleDelete() {  // ===== Botão Delete
  // Primeiro clique: apenas exibe a mensagem de confirmação
  if (!deletionConfirmation) {
    deletionConfirmation = true;
  } else { // Segundo clique: executa deleção
    // Segundo clique: executa deleção
    listCalls.removeCurrent();
    deletionConfirmation = false;
  }
}


void setup() {
  Serial.begin(115200);

  if (!connectToWiFi()) {
    Serial.println("WiFi não conectado.");
  }

  // inicializa o display
  lcd2004_init();
  // Inicializa botões
  initButtons();

  // Teste de adição de valores
  listCalls.add(10);
  listCalls.add(20);
  listCalls.add(15);
  listCalls.add(6);

  showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display
}

void loop() {

  checkAndReconnectWifi();
  delay(1000);

  // Habilita os botões somente se houver dados na lista
  if (listCalls.hasNursingCall()) {
    if (checkButton(button_next)) handleNext();
    if (checkButton(button_prev)) handlePrev();
    if (checkButton(button_delete)) handleDelete();
  }
}
