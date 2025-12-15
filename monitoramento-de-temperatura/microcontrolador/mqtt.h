#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

/**
 * @file mqtt.h
 * @brief Declarações das funções e variáveis globais relacionadas à comunicação MQTT.
 *
 * Este módulo define a interface pública para:
 * - Configuração inicial da conexão com o broker MQTT;
 * - Manutenção e reconexão automática do cliente MQTT;
 * - Publicação de dados de sensores e mensagens de alerta em formato JSON.
 * - Reenvia mensagens armazenadas localmente em caso de falha de envio anterior.
 */


// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Cliente MQTT global utilizado para comunicação com o broker.
 *
 * Este objeto é configurado e inicializado no arquivo `mqtt.cpp`,
 * utilizando um cliente seguro (`WiFiClientSecure`) como transporte.
 */
extern PubSubClient client;


// -----------------------------------------------------------------------------
// Funções públicas
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa a configuração para conexão com o servidor MQTT.
 *
 * Define parâmetros como broker, porta e credenciais.
 * Esta função deve ser chamada durante a inicialização do sistema (ex.: no `setup()`).
 */
void setupMQTT();

/**
 * @brief Verifica e garante que a conexão MQTT esteja ativa.
 *
 * - Executa `client.loop()` para manter a comunicação.
 * - Caso a conexão tenha sido perdida, tenta reconectar automaticamente
 *   dentro do intervalo de tempo configurado.
 *
 * @return true  Se o cliente MQTT estiver conectado.
 * @return false Se não foi possível restabelecer a conexão.
 */
bool checkMQTTConnected();

/**
 * @brief Publica dados de sensores (temperatura e umidade) no broker MQTT.
 *
 * O payload é enviado em formato JSON com os seguintes campos:
 * - `Microcontrollerid` — Identificador do dispositivo;
 * - `temperature` — Valor de temperatura (°C);
 * - `humidity` — Valor de umidade relativa (%).
 *
 * @param temperature Valor da temperatura a ser enviado.
 * @param humidity    Valor da umidade a ser enviado.
 * @return true  Se a publicação foi bem-sucedida.
 * @return false Se ocorreu falha na publicação.
 */
bool publishSensorData(float temperature, float humidity);

/**
 * @brief Reenvia mensagens armazenadas localmente em caso de falha de envio anterior.
 *
 * - Verifica se há dados pendentes no sistema de armazenamento;
 * - Reenvia os dados de forma periódica, respeitando o intervalo configurado;
 * - Remove mensagens já reenviadas com sucesso.
 */
void resendMqttData();

/**
 * @brief Publica uma mensagem de alerta no broker MQTT.
 *
 * O payload é enviado em formato JSON com os seguintes campos:
 * - `Microcontrollerid` — Identificador do dispositivo;
 * - `alert` — Mensagem de alerta textual.
 *
 * @param alert Mensagem de alerta a ser publicada.
 */
void publishAlert(float temperature, float humidity);


void resetStateSendAlert();

#endif
