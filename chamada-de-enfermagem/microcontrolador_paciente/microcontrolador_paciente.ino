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


      if (client.publish(TOPIC_PUBLISH, createJsonPayload(), true)) {
        Serial.println("Solicitação enviada");

        buttonBlocked = true;
        timeLastRequest = millis();


      } else {
        Serial.println("Falha ao enviar solicitação");
        buttonBlocked = false;
      }
    }
  }
}
