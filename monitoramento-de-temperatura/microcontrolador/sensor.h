#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_AHTX0.h>

/**
 * @file sensor.h
 * @brief Declarações relacionadas ao uso do sensor AHT10 (temperatura e umidade).
 * 
 * Este módulo encapsula a inicialização e a leitura de dados do sensor AHT10,
 * retornando os valores em uma estrutura própria.
 */

/**
 * @struct EnvironmentData
 * @brief Estrutura que armazena os dados ambientais coletados do sensor.
 * 
 * Contém:
 * - `temperature`: valor da temperatura em graus Celsius.
 * - `humidity`: valor da umidade relativa em porcentagem (%).
 * - `valid`: flag que indica se os dados são válidos (`true`) ou inválidos (`false`).
 */
struct EnvironmentData {
  float temperature;   ///< Temperatura em graus Celsius
  float humidity;      ///< Umidade relativa em porcentagem (%)
  bool valid;          ///< Indica se os dados lidos são válidos
};

/**
 * @brief Inicializa o sensor AHT10.
 * 
 * - Configura a comunicação I²C.
 * - Tenta iniciar o sensor AHT10.
 * - Caso a inicialização falhe, gera uma mensagem de erro via log.
 */
void initializeSensor();

/**
 * @brief Lê os dados de temperatura e umidade do sensor AHT10.
 * 
 * - Retorna uma estrutura `EnvironmentData` contendo os valores lidos.
 * - Caso os dados não sejam válidos (NaN ou valores muito baixos), o campo `valid` será `false`.
 * 
 * @return Estrutura `EnvironmentData` com temperatura, umidade e validade dos dados.
 */
EnvironmentData readSensorData();

/**
 * @brief Objeto global do sensor AHT10.
 * 
 * Declarado como `extern` para ser definido em `sensor.cpp` e acessível em outros módulos.
 */
extern Adafruit_AHTX0 aht;

#endif
