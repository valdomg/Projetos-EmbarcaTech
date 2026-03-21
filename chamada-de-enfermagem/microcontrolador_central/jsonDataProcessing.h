/**
 * @file jsonDataProcessing.h
 * @brief Interface para processamento de mensagens JSON utilizadas na comunicação MQTT.
 *
 * @details
 * Este módulo define as funções responsáveis por:
 *
 * - Interpretar mensagens JSON recebidas via MQTT
 * - Extrair informações específicas de payloads JSON
 * - Criar mensagens JSON para publicação no broker MQTT
 * - Gerar mensagens de confirmação de recebimento
 *
 * A implementação utiliza a biblioteca **ArduinoJson** para realizar
 * a serialização e desserialização dos dados JSON.
 *
 * As funções deste módulo são utilizadas principalmente na camada
 * de comunicação do sistema, intermediando os dados recebidos e
 * enviados pelo broker MQTT.
 */

#ifndef JSONDATAPROCESSING_H
#define JSONDATAPROCESSING_H

#include <Arduino.h>

struct PatientStatus {

  char id[50];

  char room_number[50];

  char estado[50];

  char mensagem[50];

  char local[50];

  char comando[50];

};



/**
 * @brief Processa um payload JSON recebido via MQTT.
 *
 * @details
 * Esta função interpreta um payload recebido do broker MQTT,
 * realiza o parse do JSON e extrai os campos necessários para
 * registrar uma nova chamada de enfermaria no sistema.
 *
 * Caso o JSON esteja inválido ou algum campo obrigatório esteja
 * ausente, a função retorna `false`.
 *
 * @param payload Ponteiro para os dados brutos recebidos via MQTT.
 * @param length Tamanho do payload recebido.
 *
 * @return true se o JSON foi processado corretamente.
 * @return false caso ocorra erro no parse ou nos dados.
 */
extern bool
processing_json_MQTT(byte* payload, unsigned int length, struct PatientStatus *patientStatus);


/**
 * @brief Cria um payload JSON para envio via MQTT.
 *
 * @details
 * Esta função gera um JSON contendo informações do dispositivo
 * e da enfermaria associada a uma ação.
 *
 * O JSON gerado normalmente é utilizado para indicar o estado
 * atual do dispositivo e enviar comandos ou respostas para o
 * broker MQTT.
 *
 * @param buffer Buffer onde o JSON serializado será armazenado.
 * @param length Tamanho máximo do buffer.
 * @param roomNumber Número da enfermaria associado à mensagem.
 *
 * @return Ponteiro para o buffer contendo o JSON serializado.
 */
const char* createJsonPayload(char* buffer, size_t length, const char* command);


/**
 * @brief Extrai o campo "id" de um payload JSON.
 *
 * @details
 * Utilizado quando é necessário identificar rapidamente qual
 * dispositivo ou entidade enviou uma mensagem MQTT.
 *
 * A função realiza o parse do JSON e retorna o valor do campo
 * `"id"`.
 *
 * @param payload Ponteiro para os dados recebidos via MQTT.
 * @param length Tamanho do payload.
 *
 * @return Ponteiro para string contendo o ID extraído do JSON.
 */
const char* getPayloadID(byte* payload, unsigned int length);


/**
 * @brief Cria um payload JSON de confirmação de mensagem.
 *
 * @details
 * Esta função gera um JSON simples indicando que uma mensagem
 * foi recebida e processada com sucesso.
 *
 * Exemplo de JSON gerado:
 *
 * {
 *   "status": "ok"
 * }
 *
 * Esse payload pode ser enviado para um tópico MQTT de confirmação.
 *
 * @param buffer Buffer onde o JSON será armazenado.
 * @param length Tamanho máximo do buffer.
 *
 * @return Ponteiro para o buffer contendo o JSON serializado.
 */
const char* creteJsonPayloadConfirmationMessage(char* buffer, size_t length);

#endif