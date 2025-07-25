// Código para leitura de temperatura e umidade usando o sensor AHT10 com ESP32
// Os dados são exibidos no monitor serial a cada 3 segundos

#include <Wire.h>
#include "Adafruit_AHTX0.h"

Adafruit_AHTX0 aht;

struct EnvironmentData {      //Estrutura para desacoplamento do sensor
  float temperature;        
  float humidity;
  bool valid;
};

EnvironmentData readDataSensor() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  EnvironmentData data;
  if (isnan(temp.temperature) || isnan(hum.relative_humidity)) {
    data.valid = false;
    return data;
  }

  data.temperature = temp.temperature;
  data.humidity = hum.relative_humidity;
  data.valid = true;

  return data;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin(21, 22);

  Serial.println("Iniciando sensor AHT10 com Adafruit AHTX0...");

  if (!aht.begin()) {
    Serial.println("Não foi possível encontrar o AHT10! Verifique a fiação.");
    while (1) delay(10);
  }

  Serial.println("Sensor AHT10 detectado com sucesso!");
}

void loop() {

  EnvironmentData sensorReading = readDataSensor();

  if (sensorReading.valid) {
    Serial.print("Temperatura: ");
    Serial.print(sensorReading.temperature);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(sensorReading.humidity);
    Serial.println(" %");
  }


  delay(3000);
}
