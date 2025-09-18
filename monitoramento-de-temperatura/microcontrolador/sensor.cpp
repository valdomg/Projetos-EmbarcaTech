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
    log(LOG_ERROR, "Sensor AHT10 não inicializado");
  } else {
    log(LOG_DEBUG, "Sensor iniciado");
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
  data.temperature = temp.temperature;
  data.humidity = hum.relative_humidity;

  if (isnan(temp.temperature) || isnan(hum.relative_humidity) || temp.temperature <= 0.5 || hum.relative_humidity <= 0.5 ) {
    data.valid = false;
    return data;
  }

  data.valid = true;

  return data;
}
