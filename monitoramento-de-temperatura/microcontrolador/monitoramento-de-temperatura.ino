#include "sensor.h"
#include "wifi.h"
#include "mqtt.h"
#include "display_LCD-1602_I2C.h"

unsigned long previousMQTTTime = 0;
const unsigned long MQTTInterval = 180000;  // 3 minutos

unsigned long previousSensorReading = 0;
unsigned long sensorRedingInterval = 1000;  // 1 segundo

void setup() {
  Serial.begin(115200);
  initializeSensor();

  // Chama a função para iniciar o display
  initializeDisplayLCD_1602_I2C();

  // if (!connectWiFi()) {
  //   Serial.println("WiFi não conectado.");
  // }
  // setupMQTT();
}

void loop() {
  // client.loop();
  // checkMQTTConnected();

  unsigned long now = millis();
  if (now - previousSensorReading >= sensorRedingInterval) {
    EnvironmentData data = readSensorData();

    if (!data.valid) {
      //logica se o sensor não funcionar
    }

    //logica para limite de temperatura

    Serial.println("==== valor mostrado no display ====");
    printSensorData(data);


    // Passa os dados e os status de alerta para a função de exibição
    show_data_tempHum_DisplayLCD_1602_I2C(data.temperature, data.humidity, false, false);
    Serial.println("===================================\n");

    previousSensorReading = now;

    // Logica do display

    // if (now - previousMQTTTime >= MQTTInterval) {
    //   previousMQTTTime = now;
    //   publishSensorData(data.temperature, data.humidity);
    //   Serial.println("==== valor enviado para o MQTT ====");
    //   printSensorData(data);
    //   Serial.println("===================================\n");
    // }
  }
}
