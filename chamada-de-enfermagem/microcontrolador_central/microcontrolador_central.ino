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
void handleNext() {  // ===== Botão Next (>)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    fixed_data();                  // Atualiza o display com os dados fixos
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)
  } else {
    // Avança para o próximo item da lista
    listCalls.next();
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}

void handlePrev() {  // ===== Botão Prev (<)
  // Se estava em modo confirmação de deleção, atualiza o display, mostrando que a ação foi cancelada
  if (deletionConfirmation) {
    fixed_data();                  // Atualiza o display com os dados fixos
    deletionConfirmation = false;  // reset caso usuário navegue (cancela a exclusão)
  } else {
    // Avança para o item anteriot da lista
    listCalls.prev();
  }
  // Mostra o item atual
  showInfirmaryNumber(
    listCalls.getInfirmaryCurrent(),
    listCalls.hasNursingCall(),
    listCalls.getTotal());
}


void handleDelete() {  // ===== Botão Delete
  // Primeiro clique: apenas exibe a mensagem de confirmação
  if (!deletionConfirmation) {
    showExclusionConfirm(listCalls.getInfirmaryCurrent());
    deletionConfirmation = true;
  } else {                      // Segundo clique: executa deleção
    listCalls.removeCurrent();  // Apaga o item selecionado
    fixed_data();               // Atualiza o display com os dados fixos
    showInfirmaryNumber(
      listCalls.getInfirmaryCurrent(),
      listCalls.hasNursingCall(),
      listCalls.getTotal());  // Mostra os dados no display
      // reseta o flag
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
