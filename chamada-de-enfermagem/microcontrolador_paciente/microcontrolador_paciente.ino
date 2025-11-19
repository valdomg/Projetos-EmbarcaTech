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

  //se estiver conectado no broker entra no loop de leitura do botao
  if (client.connected()) {
    //se o botao estiver false e for pressionado publica no broker
    if (!buttonBlocked && !readButton()) {
      delay(50);
      if (client.publish(TOPIC_PUBLISH, createJsonPayload())) {
        Serial.println("Solicitação enviada");
      } else {
        Serial.println("Falha ao enviar solicitação");
      }
    }
  } 
}
