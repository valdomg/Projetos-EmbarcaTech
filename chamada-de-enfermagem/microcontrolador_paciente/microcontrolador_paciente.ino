#include "wifi.h"
#include "mqtt.h"
#include "button.h"
#include "led.h"

bool buttonBlocked = false;

void setup() {

  Serial.begin(115200);
  setupButton();
  setupLed();
  connectWiFi();
  connectMQTT();

}

void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
  checkConnection();

  //se o botao for pressionado, ficará indisponível até que o enfermeiro libere o recurso novamente
  if (!buttonBlocked && !readButton()) {
      Serial.println("Botão pressionado!");

      ligarLed();
      publishData();
      buttonBlocked = true;
    }
  }