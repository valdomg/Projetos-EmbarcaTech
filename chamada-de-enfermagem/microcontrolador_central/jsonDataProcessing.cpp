#include "jsonDataProcessing.h"
#include "listNursingCall_utils.h"
#include <ArduinoJson.h>
#include <cstring>


/*  Exemplo JSON (payload) recebido:
{
  "id": "Enfermagem1",
  "estado": "emergencia",
  "mensagem": "Ligar LED",
  "room_number": "9",
  "local": "Enfermaria",
  "comando": "ligar"
}
*/
// processa os dados JSON recebido do MQTT
void processing_json_MQTT(byte* payload, unsigned int length){
  // Cria um documento JSON estático na stack (memória temporária) com capacidade de 256 bytes para armazenar o JSON parseado.
  StaticJsonDocument<256> doc;  // Ou <512> 

  // Tenta converter (parsear) os dados brutos em estrutura JSON
  DeserializationError error = deserializeJson(doc, payload, length);

  // Verifica se ocorreu algum erro no parse do JSON
  if (error) {
    Serial.print(F("Erro no parse: "));
    // converte o erro em string legível e a imprimi no monitor serial
    Serial.println(error.f_str());
    // Sai da função prematuramente se houve erro no parse
    return;
  }

  // ____Extração dos Dados do JSON
  // Acessa o campo "id" do JSON e armazena como ponteiro para string constante
  const char* id = doc["id"]; // Ex: "Enfermagem1"
  // Extrai campo "estado" do JSON
  const char* estado = doc["estado"]; // Ex: "emergencia"
  // Extrai campo "mensagem" do JSON
  const char* mensagem = doc["mensagem"]; // Ex: "Ligar LED"


  // // Extrai campo "mensagem" do JSON
  // const char* room_number = doc["room_number"]; 

  // *** CONVERSÃO PARA INTEIRO ***
  // Declara e inicializa a variável 'room_number' com 0
  int room_number = 0;
  // Verifica se o campo "room_number" do objeto 'doc' é um número inteiro
  if (doc["room_number"].is<int>()) {
    // Se for inteiro, converte diretamente para int
    room_number = doc["room_number"].as<int>();
  } else if (doc["room_number"].is<const char*>()) { // Caso contrário, verifica se o campo é string (const char*)
    // Obtém a string do campo "room_number"
    const char* rn = doc["room_number"];
    // Ponteiro usado por strtol() para identificar onde a conversão parou
    char* endptr = nullptr;
    // Converte a string para número inteiro longo (base 10)
    long v = strtol(rn, &endptr, 10);

    // Verifica se a conversão falhou:
    // - Se endptr == rn, nenhum dígito foi convertido.
    // - Se *endptr != '\0', há caracteres não numéricos após o número.
    if (endptr == rn || *endptr != '\0') {
      Serial.println("Erro: room_number string inválida");
      return; // não adiciona o dado (saída antecipada)
    }

    // Converte o valor long obtido para int
    room_number = (int)v;

  } else { // Caso o tipo de 'room_number' não seja nem inteiro nem string
    Serial.println("Erro: room_number não é string nem número!");
    return; // não adiciona o dado
  }
  // Verificação adicional: garante que o número esteja dentro do intervalo aceitável (Modificar de acordo com o números dos quartos)
  if (room_number <= 0 || room_number > 9999) {
    Serial.println("room_number fora dos limites aceitáveis");
    return; // Valor inválido, interrompe a execução
  }


  // Extrai campo "mensagem" do JSON
  const char* local = doc["local"]; // Ex: "Enfermagem"
  // Extrai campo "comando" do JSON
  const char* comando = doc["comando"]; // Ex: "ligar"

  // ____Exibição dos Dados
  Serial.println("\n#######################");

  Serial.print(" -> ID: ");
  Serial.println(id);

  Serial.print(" -> estado: ");
  Serial.println(estado);

  Serial.print(" -> mensagem: ");
  Serial.println(mensagem);

  Serial.print(" (--> Num. enfermaria: ");
  Serial.println(room_number);

  Serial.print(" -> local: ");
  Serial.println(local);

  Serial.print(" -> comando: ");
  Serial.println(comando);

  Serial.println("#######################");

  // Adiciona na lista somente os valores importantes para marcar o chamado como resolvido
  listCalls.add(room_number, id);
  listUpdated = true;  // marca que a lista foi alterada
}