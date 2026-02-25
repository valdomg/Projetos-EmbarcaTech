/**
 * @file sensor.cpp
 * @brief Implementação do módulo de leitura do sensor ambiental AHT10.
 *
 * Este módulo é responsável por inicializar e realizar a leitura dos dados
 * de temperatura e umidade relativa do ar utilizando o sensor AHT10 através
 * da interface de comunicação I²C.
 *
 * Funcionalidades principais:
 *
 * - Inicialização do barramento I²C;
 * - Inicialização do sensor AHT10;
 * - Leitura periódica dos dados ambientais;
 * - Validação dos dados lidos;
 * - Fornecimento dos dados através da estrutura EnvironmentData.
 *
 * Os dados coletados são utilizados por outros módulos do sistema, como:
 *
 * - Exibição no display LCD;
 * - Envio para o broker MQTT;
 * - Verificação de condições de alerta;
 * - Armazenamento local.
 *
 * Comunicação:
 *
 * - Interface: I²C
 * - Pinos utilizados:
 *      SDA → definido em config.h
 *      SCL → definido em config.h
 *
 * Biblioteca utilizada:
 *
 * - Adafruit AHTX0
 *
 * Estrutura de dados retornada:
 *
 * @code
 * struct EnvironmentData {
 *     float temperature;
 *     float humidity;
 *     bool valid;
 * };
 * @endcode
 *
 * @note Este módulo depende da inicialização correta do barramento I²C.
 *
 * @warning Caso o sensor não seja inicializado corretamente, os dados retornados serão inválidos.
 *
 * @see sensor.h
 */

#include <Wire.h>
#include "sensor.h"
#include "log.h"
#include "config.h"

Adafruit_AHTX0 aht;  ///< Objeto global do sensor AHT10 (via biblioteca Adafruit)

/**
 * @brief Inicializa o sensor AHT10.
 * 
 * - Configura a comunicação I²C nos pinos definidos (SDA = 4, SCL = 5).
 * - Tenta iniciar o sensor AHT10 utilizando a biblioteca Adafruit.
 * - Caso não consiga inicializar, registra mensagem de erro no log.
 * - Caso seja bem-sucedido, registra mensagem de debug.
 * 
 * @note Em versões futuras, os pinos podem ser passados como parâmetros em vez de fixos.
 */
void initializeSensor() {
  Wire.begin(PIN_SENSOR_SDA, PIN_SENSOR_SCL);  // Define os pinos SDA e SCL do barramento I²C

  if (!aht.begin()) {
    log(LOG_ERROR, "Sensor AHT10 não inicializado");
  } else {
    log(LOG_DEBUG, "Sensor iniciado");
  }
}

/**
 * @brief Lê os dados de temperatura e umidade do sensor AHT10.
 * 
 * - Captura os eventos de temperatura e umidade através da biblioteca Adafruit.
 * - Armazena os valores na estrutura `EnvironmentData`.
 * - Realiza validações para garantir que os dados são numéricos e plausíveis.
 * 
 * @return Estrutura `EnvironmentData` contendo:
 *   - `temperature` → Temperatura em graus Celsius.
 *   - `humidity` → Umidade relativa em %.
 *   - `valid` → `true` se os dados são válidos, `false` caso contrário.
 */
EnvironmentData readSensorData() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);  // Coleta dados do sensor

  EnvironmentData data;
  data.temperature = temp.temperature;
  data.humidity = hum.relative_humidity;

  // Verifica se os dados são válidos (não NaN e maiores que valores mínimos)
  if (isnan(temp.temperature) || isnan(hum.relative_humidity) || 
      temp.temperature <= 0.5 || hum.relative_humidity <= 0.5) {
    data.valid = false;
    return data;
  }

  data.valid = true;
  return data;
}
