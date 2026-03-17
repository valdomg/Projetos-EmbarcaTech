/**
 * @file mqtt.h
 * @brief Interface do módulo de comunicação MQTT.
 *
 * @details
 * Define as funções e variáveis responsáveis pela
 * comunicação com o broker MQTT no sistema de
 * chamada de enfermagem.
 */

#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

/**
 * @defgroup MQTTModule Módulo MQTT
 * @brief Funções e definições relacionadas à comunicação MQTT.
 * @{
 */

/**
 * @brief Cliente MQTT global.
 *
 * Utilizado para publicação e inscrição em tópicos.
 */
extern PubSubClient client;

/**
 * @brief Tópico utilizado para publicar solicitações do dispositivo.
 */
extern const char* TOPIC_PUBLISH;

/**
 * @brief Cria o payload JSON da solicitação de atendimento.
 *
 * @return Ponteiro para string contendo JSON serializado.
 */
const char* createJsonPayload();

/**
 * @brief Inicializa a configuração do cliente MQTT.
 *
 * Define servidor e função de callback.
 */
void setupMQTT();

/**
 * @brief Realiza tentativa de conexão com o broker MQTT.
 *
 * Implementa controle de reconexão com intervalo mínimo.
 */
void connectMQTT();

/**
 * @brief Função de callback executada ao receber mensagem MQTT.
 *
 * @param topic   Tópico onde a mensagem foi recebida.
 * @param payload Conteúdo da mensagem recebida.
 * @param length  Tamanho do payload.
 *
 * Responsável por interpretar mensagens JSON
 * e executar ações no dispositivo (ex: controle do LED).
 */
void callback(char *topic, byte *payload, unsigned int length);

/** @} */ // fim do grupo MQTTModule

#endif // MQTT_H