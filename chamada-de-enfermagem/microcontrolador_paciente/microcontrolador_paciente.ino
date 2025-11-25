#include "wifi.h"
#include "mqtt.h"
#include "button.h"
#include "led.h"

bool buttonBlocked = false;
bool confirmMsg = false;

unsigned long timeLastRequest = 0;
const unsigned long TIMEOUT_ACK = 6000;
const int MAX_RETRY = 10;
int retryCount = 0;

void setup() {

  Serial.begin(115200);
  setupButton();
  setupLed();
  setupMQTT();
  connectWiFi();
}


void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
  checkConnection();

  //se estiver conectado no broker entra no loop de leitura do botao
  if (client.connected()) {
    //se o botao estiver liberado e for pressionado, publica no broker
    if (!buttonBlocked && !readButton()) {


      retryCount = 0;
      confirmMsg = false;

      if (client.publish(TOPIC_PUBLISH, createJsonPayload())) {
        Serial.println("Solicitação enviada");
        buttonBlocked = true;
        timeLastRequest = millis();


      } else {
        Serial.println("Falha ao enviar solicitação");
      }
    }
  }

  // Controle de timeout e tentativas de reenviar
  if (!confirm && millis() - timeLastRequest > TIMEOUT_ACK) {

    if (retryCount < MAX_RETRY) {
      retryCount++;
      Serial.printf("Tentando novamente... (%d/%d)\n", retryCount, MAX_RETRY);

      client.publish(TOPIC_PUBLISH, createJsonPayload());

      timeLastRequest = millis();

    } else {
      buttonBlocked = false;
      confirm = true;
      retryCount = 0;
    }
  }
}
