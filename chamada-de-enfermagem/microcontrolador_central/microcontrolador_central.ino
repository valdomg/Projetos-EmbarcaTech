#include "wifi_utils.h"

#include "display_LCD-2004_I2C.h"
#include "DoublyLinkedList_NursingCall.h"
#include "buttons.h"


List_NursingCall listCalls;


// ===== Funções de navegação =====
void handleNext() {
  listCalls.next();
  Serial.printf("Clicou next %d \n", listCalls.getInfirmaryCurrent());
}

void handlePrev() {
  listCalls.prev();
  Serial.printf("Clicou prev %d \n", listCalls.getInfirmaryCurrent());
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
  }
}
