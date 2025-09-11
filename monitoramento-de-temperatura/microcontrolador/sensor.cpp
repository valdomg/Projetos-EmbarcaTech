#include <Wire.h>
#include "sensor.h"
#include "log.h"

Adafruit_AHTX0 aht;

/**
 * Inicializa o sensor AHT10.
 */
void initializeSensor() {
  Wire.begin(4, 5);  // Pinos fixos aqui ou passados por parâmetro depois

  if (!aht.begin()) {
    log(LOG_ERROR,"Sensor AHT10 não inicializado");
    while (1) delay(10);
  }
}


/**
 * Lê os dados de temperatura e umidade do sensor AHT10.
 * 
 * @return Estrutura contendo temperatura (°C), umidade (%) e status de validade dos dados.
 */
EnvironmentData readSensorData() {
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

