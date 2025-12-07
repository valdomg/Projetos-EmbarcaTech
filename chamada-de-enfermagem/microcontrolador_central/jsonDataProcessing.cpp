#include "jsonDataProcessing.h"
#include "listNursingCall_utils.h"
#include <ArduinoJson.h>
#include <cstring>
#include "log.h"
#include "config.h"


/*  Exemplo JSON (payload) recebido:
{
  "id": "Enfermagem1",
  "estado": "emergencia",
  "mensagem": "Ligar LED",
  "room_number": "9",
  "local": "Enfermaria",
  "comando": "ligar"
}*/


// Capacidade do StaticJsonDocument
static constexpr size_t JSON_CAPACITY = 256;


// processa os dados JSON recebido do MQTT
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
  if (!doc["room_number"].is<const char*>()) { // verifica se o campo existir e se é uma string JSON
    log(LOG_ERROR, "Campo 'room_number' ausente ou não é string");
    return false;
  }
  const char* room_number = doc["room_number"];  // Ex: "A4H143"
  if (room_number[0] == '\0') { // verifica se a string está vazia
    log(LOG_ERROR, "Campo 'room_number' vazio");
    return false;
  }

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
      " -> Num. enfermaria: %d\n"
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


const char* createJsonPayload(char* buffer, size_t length, const char* roomNumber) {
  StaticJsonDocument<256> doc;

  doc["id"] = MQTT_DEVICE_ID;  //ID do proprio dispositivo
  doc["estado"] = "ocioso";
  doc["mensagem"] = "Desligar LED";
  doc["room_number"] = roomNumber;
  doc["local"] = "posto_enfermaria";
  doc["comando"] = "desligar";

  serializeJson(doc, buffer, length);

  return buffer;
}


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

const char* creteJsonPayloadConfirmationMessage(char* buffer, size_t length){
  StaticJsonDocument<50> doc;

  doc["status"] = "ok"; 

  serializeJson(doc, buffer, length);

  return buffer;
}