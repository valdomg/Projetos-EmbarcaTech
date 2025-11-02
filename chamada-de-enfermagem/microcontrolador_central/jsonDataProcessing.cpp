#include "jsonDataProcessing.h"
#include "listNursingCall_utils.h"
#include <ArduinoJson.h> // biblioteca que faz parse/serialização JSON


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


// Converte string para número (Retorna true se conversão e limites estiverem ok e grava resultado em room_number)
//    int& room_number: referência para variável que receberá o resultado
//    const char* room_str: string a ser convertida
bool roomNumberConversion(int& room_number, const char* room_str) {
  if (room_str == nullptr) return false;  // Verifica se string é nula - retorna falha

  char* endptr = nullptr;                  // ponteiro que indica onde a conversão parou
  long v = strtol(room_str, &endptr, 10);  // conversão string para long

  // Verifica se conversão foi completa
  if (endptr == room_str) {
    Serial.println(F("Erro: room_number string inválida"));
    return false;
  }

  // Há caracteres extras após número
  if (*endptr != '\0') {
    Serial.println(F("Erro: room_number string inválida (caracteres extras)"));
    return false;
  }

  room_number = static_cast<int>(v);  // Atribui valor convertido via referência
  return true; // Retorna sucesso - conversão válida
}


// Validação básica do payload JSON bruto
bool validateJsonFields(const char* payload, unsigned int length) {
  if (payload == nullptr) return false;
  if (length == 0) return false;

  if (length > 1024) {  // limite de segurança
    Serial.println(F("Erro: Payload muito grande!"));
    return false;
  }

  // Checa formato mínimo - deve começar com '{'
  if (payload[0] != '{') {
    Serial.println(F("Erro: Payload não parece JSON"));
    return false;
  }

  return true;  // Payload é válido
}


// processa os dados JSON recebido do MQTT
void processing_json_MQTT(byte* payload, unsigned int length){
  // Converte o payload (byte array) para const char* sem copiar
  const char* p = reinterpret_cast<const char*>(payload);

  // Valida formato e tamanho
  if (!validateJsonFields(p, length)) {
    Serial.println(F("Erro: Payload inválido"));
    return;  // Se inválido: Imprime erro e sai da função
  }

  // Cria documento JSON estático na stack (memória temporária) para armazenar o JSON parseado.
  StaticJsonDocument<JSON_CAPACITY> doc;  //

  // Tenta converter (parsear) os dados brutos em estrutura JSON
  DeserializationError error = deserializeJson(doc, p, length);

  // Verifica se ocorreu algum erro no parse do JSON
  if (error) {
    Serial.print(F("Erro no parse: "));
    // converte o erro em string legível e a imprimi no monitor serial
    Serial.println(error.f_str());
    // Sai da função prematuramente se houve erro no parse
    return;
  }

  // --- leitura de id ---
  if (!doc["id"].is<const char*>()) { // só passa se o campo existir e for uma string JSON
    Serial.println(F("Erro: Campo 'id' ausente ou não é string"));
    return;
  }
  const char* id = doc["id"];  // Ex: "Enfermagem1"
  if (id[0] == '\0') { // verifica se a string está vazia
    Serial.println(F("Erro: Campo 'id' vazio"));
    return;
  }


  // --- leitura e conversão de room_number ---
  int room_number = 0; // Declara e inicializa a variável 'room_number' com 0
  bool roomNumberValid = false;  // flag indicando se conversão foi bem-sucedida

  // Verifica se o campo "room_number" do objeto 'doc' é um número inteiro
  if (doc["room_number"].is<int>()) {
    // Se for inteiro, converte diretamente para int
    room_number = doc["room_number"].as<int>();
    roomNumberValid = true; // Marca como válido

  } 
  // se tipo é string, converter
  else if (doc["room_number"].is<const char*>()) {  // Caso contrário, verifica se o campo é string (const char*)
    // Obtém a string do campo "room_number"
    const char* rn = doc["room_number"];
    // Extrai string e chama função de conversão
    roomNumberValid = roomNumberConversion(room_number, rn);

  } else {  // Caso o tipo de 'room_number' não seja nem inteiro nem string
    Serial.println(F("Erro: room_number não é string nem número!"));
    return;  // não adiciona o dado
  }

  if (!roomNumberValid) {  // Verifica se conversão foi bem-sucedida
    return; // Se falhou, sai da função
  }


  // ____Exibição dos Dados
  Serial.println(F("\n#######################"));

  Serial.print(F(" -> ID: "));
  Serial.println(id);
  Serial.print(F(" (--> num quarto: "));
  Serial.println(room_number);

  Serial.println(F("#######################"));


  // Adiciona na lista com verificação de sucesso
  if (listCalls.add(room_number, id)) {
    listUpdated = true;  // sinaliza que display precisa atualizar
    Serial.println(F("Chamada adicionada com sucesso!"));
  } else {
    Serial.println(F("Erro ao adicionar chamada na lista!"));
  }
}