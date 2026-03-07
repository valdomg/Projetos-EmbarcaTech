/**
 * @file jsonDataProcessing.cpp
 * @brief Processamento de mensagens JSON recebidas e enviadas via MQTT.
 *
 * @details
 * Este módulo é responsável por:
 *
 * - Interpretar mensagens JSON recebidas pelo broker MQTT
 * - Extrair campos relevantes das mensagens
 * - Inserir novas chamadas na lista de chamadas de enfermaria
 * - Criar mensagens JSON para publicação MQTT
 *
 * A biblioteca utilizada para manipulação de JSON é a ArduinoJson,
 * que permite parse e serialização de JSON de forma eficiente em
 * sistemas embarcados.
 */

#include "jsonDataProcessing.h"
#include "listNursingCall_utils.h"
#include <ArduinoJson.h>
#include <cstring>
#include "log.h"
#include "config.h"
#include "config_storage.h"


/**
 * @example Exemplo de JSON recebido via MQTT
 *
 * {
 *   "id": "Enfermagem1",
 *   "estado": "emergencia",
 *   "mensagem": "Ligar LED",
 *   "room_number": "9",
 *   "local": "Enfermaria",
 *   "comando": "ligar"
 * }
 */


/**
 * @brief Capacidade máxima do documento JSON utilizado no parse.
 *
 * @details
 * Define o tamanho máximo do buffer usado pelo ArduinoJson para
 * armazenar temporariamente os dados do JSON parseado.
 *
 * Este valor deve ser ajustado caso a estrutura do JSON aumente.
 */
static constexpr size_t JSON_CAPACITY = 256;


/**
 * @brief Processa um payload JSON recebido via MQTT.
 *
 * @details
 * Esta função:
 *
 * 1. Converte o payload recebido em um objeto JSON.
 * 2. Valida os campos obrigatórios do JSON.
 * 3. Extrai as informações relevantes.
 * 4. Insere a chamada na lista de chamadas de enfermaria.
 *
 * Caso ocorra erro no parse ou em algum campo obrigatório,
 * a função retorna `false`.
 *
 * @param payload Ponteiro para os dados recebidos pelo MQTT.
 * @param length Tamanho do payload recebido.
 *
 * @return true se o processamento foi realizado com sucesso.
 * @return false caso ocorra erro.
 */
bool processing_json_MQTT(byte* payload, unsigned int length) {
  // Cria um documento JSON estático na stack (memória temporária) com capacidade de 256 bytes para armazenar o JSON parseado.
  StaticJsonDocument<JSON_CAPACITY> doc;  // Ou <512>

  // Tenta converter (parsear) os dados brutos em estrutura JSON
  DeserializationError error = deserializeJson(doc, payload, length);

  // Verifica se ocorreu algum erro no parse do JSON
  if (error) {
    log(LOG_ERROR, "Erro no parse: ");
    // converte o erro em string legível e a imprimi no monitor serial
    log(LOG_ERROR, error.c_str());
    // Sai da função prematuramente se houve erro no parse
    return false;
  }

  // ____Extração dos Dados do JSON
  // Acessa o campo "id" do JSON e armazena como ponteiro para string constante
  // --- leitura de id ---
  if (!doc["id"].is<const char*>()) { // verifica se o campo existir e se é uma string JSON
    log(LOG_ERROR, "Campo 'id' ausente ou não é string");
    return false;
  }
  const char* id = doc["id"];  // Ex: "Enfermagem1"
  if (id[0] == '\0') { // verifica se a string está vazia
    log(LOG_ERROR, "Campo 'id' vazio");
    return false;
  }

  // --- leitura de room_number ---
  JsonVariant rn = doc["room_number"];
  if (rn.isNull()) {
    log(LOG_ERROR, "Campo 'room_number' ausente");
    return false;
  }

  /**
   * @note
   * Conversão temporária para String.
   * Utiliza variável estática para manter o buffer válido após a função.
   */
  static String room_number_str; 
  room_number_str = rn.as<String>();

  if (room_number_str.length() == 0) { // verifica se a string está vazia
    log(LOG_ERROR, "Campo 'room_number' vazio");
    return false;
  }

  // Obtem const char* seguro (aponta para buffer interno estático)
  const char* room_number = room_number_str.c_str();

  // Extrai campo "estado" do JSON
  const char* estado = doc["estado"];  // Ex: "emergencia"
  // Extrai campo "mensagem" do JSON
  const char* mensagem = doc["mensagem"];  // Ex: "Ligar LED"
  // Extrai campo "mensagem" do JSON
  const char* local = doc["local"];  // Ex: "Enfermagem"
  // Extrai campo "comando" do JSON
  const char* comando = doc["comando"];  // Ex: "ligar"

  log(LOG_INFO,
      " -> ID: %s\n"
      " -> estado: %s\n"
      " -> mensagem: %s\n"
      " -> Num. enfermaria: %s\n"
      " -> local: %s\n"
      " -> comando: %s\n",
      id,
      estado,
      mensagem,
      room_number,
      local,
      comando);


  // Adiciona na lista com verificação de sucesso
  if (listCalls.add(room_number, id)) {
    listUpdated = true;  // sinaliza que display precisa atualizar
    return true;
  } else {
    log(LOG_ERROR, "Erro ao adicionar chamada na lista!");
    return false;
  }
}

/**
 * @brief Cria payload JSON para envio via MQTT.
 *
 * @details
 * Esta função gera uma mensagem JSON contendo informações
 * do dispositivo que está respondendo à chamada.
 *
 * O JSON gerado possui o formato:
 *
 * {
 *   "id": "<device_id>",
 *   "estado": "ocioso",
 *   "mensagem": "Desligar LED",
 *   "room_number": "...",
 *   "local": "posto_enfermaria",
 *   "comando": "desligar"
 * }
 *
 * @param buffer Buffer onde o JSON será armazenado.
 * @param length Tamanho máximo do buffer.
 * @param roomNumber Número da enfermaria relacionada à mensagem.
 *
 * @return Ponteiro para o buffer contendo o JSON serializado.
 */
const char* createJsonPayload(char* buffer, size_t length, const char* roomNumber) {
  StaticJsonDocument<256> doc;

  doc["id"] = cfg.mqttDeviceId.c_str(); //ID do proprio dispositivo
  doc["estado"] = "ocioso";
  doc["mensagem"] = "Desligar LED";
  doc["room_number"] = roomNumber;
  doc["local"] = "posto_enfermaria";
  doc["comando"] = "desligar";

  serializeJson(doc, buffer, length);

  return buffer;
}

/**
 * @brief Extrai apenas o campo "id" de um payload JSON.
 *
 * @details
 * Utilizado quando é necessário identificar rapidamente
 * qual dispositivo enviou uma mensagem MQTT.
 *
 * @param payload Dados recebidos do MQTT.
 * @param length Tamanho do payload.
 *
 * @return Ponteiro para string contendo o ID extraído
 * ou mensagem de erro caso o parse falhe.
 */
const char* getPayloadID(byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    log(LOG_ERROR, "Erro no parse: ");
    // converte o erro em string legível e a imprimi no monitor serial
    log(LOG_ERROR, error.c_str());
    // Sai da função prematuramente se houve erro no parse
    return error.c_str();
  }
  const char* id = doc["id"].as<const char*>();

  return id;
}

/**
 * @brief Cria payload JSON de confirmação de mensagem.
 *
 * @details
 * Utilizado para confirmar o recebimento de uma mensagem MQTT.
 *
 * JSON gerado:
 *
 * {
 *   "status": "ok"
 * }
 *
 * @param buffer Buffer onde o JSON será armazenado.
 * @param length Tamanho máximo do buffer.
 *
 * @return Ponteiro para o buffer contendo o JSON serializado.
 */
const char* creteJsonPayloadConfirmationMessage(char* buffer, size_t length){
  StaticJsonDocument<50> doc;

  doc["status"] = "ok"; 

  serializeJson(doc, buffer, length);

  return buffer;
}