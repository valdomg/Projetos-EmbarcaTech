#include "wifi.h"
#include "mqtt.h"
#include "button.h"
#include "led.h"

// True quando o botão está habilitado para enviar uma solicitação
bool buttonEnable = false;



void setup() {
 // Inicializa comunicação serial para debug
  Serial.begin(115200);

 // Inicializa periféricos de entrada e saída
  setupButton();
  setupLed();

  // Inicializa cliente MQTT e conecta ao Wi-Fi
  setupMQTT();
  connectWiFi();
}


void loop() {

  // Garante que o cliente MQTT esteja conectado
  if (!client.connected()) {
    connectMQTT();
  }

// Processa o loop interno do cliente MQTT
  client.loop();

  // Verifica estado da conexão Wi-Fi
  checkConnection();

   // Executa a lógica principal apenas quando conectado ao broker MQTT
  if (client.connected()) {

    // Se o botão estiver liberado e for pressionado, envia uma solicitação para o broker MQTT
    if (!buttonEnable && !readButton()) {

      // Publica a mensagem no tópico configurado
      if (client.publish(TOPIC_PUBLISH, createJsonPayload(), true)) {
        Serial.println("Solicitação enviada");
        
        // Bloqueia o botão após envio bem-sucedido, para evitar múltiplos publishes consecutivos
        buttonEnable = true;


      } else {
        Serial.println("Falha ao enviar solicitação");

        // Mantém o botão liberado em caso de falha no envio
        buttonEnable = false;
      }
    }
  }
}
