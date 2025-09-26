#include "storage.h"
#include "log.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

// -----------------------------------------------------------------------------
// Definições e variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Caminho do arquivo de armazenamento no sistema de arquivos LittleFS.
 */
const char* PATH = "/storage.json";

/**
 * @brief Flag que indica se há algum objeto armazenado no sistema.
 */
bool hasObj = false;

// -----------------------------------------------------------------------------
// Funções de inicialização e escrita
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o sistema de arquivos LittleFS.
 * 
 * - Monta a partição LittleFS.
 * - Caso a montagem falhe, gera um log de erro.
 */
void initStorage() {
  if (!LittleFS.begin()) {
    log(LOG_ERROR, "Erro ao montar LittleFS");
    return;
  }
}

/**
 * @brief Salva um novo objeto (temperatura e umidade) no arquivo JSON.
 * 
 * - Abre o arquivo em modo append (`"a"`).
 * - Cria um documento JSON com os valores fornecidos.
 * - Grava os dados no arquivo e fecha.
 * - Atualiza a flag `hasObj`.
 * 
 * @param temperature Valor da temperatura a ser armazenada.
 * @param humidity    Valor da umidade a ser armazenada.
 */
void saveStorage(float temperature, float humidity) {

  File file = LittleFS.open(PATH, "a");
  if (!file) {
    log(LOG_ERROR, "Erro ao abrir arquivo não existente");
    return;
  }

  StaticJsonDocument<128> doc;
  doc["t"] = temperature;
  doc["h"] = humidity;

  serializeJson(doc, file);
  file.println();
  file.close();
  
  hasObj = true;
  log(LOG_INFO, "Mensagem salva");
}

// -----------------------------------------------------------------------------
// Funções de leitura
// -----------------------------------------------------------------------------

/**
 * @brief Recupera um objeto armazenado em uma linha específica do arquivo JSON.
 * 
 * - Cada linha do arquivo representa um objeto JSON.
 * - A leitura é feita até alcançar o índice informado.
 * - Caso os dados sejam válidos, retorna um objeto preenchido.
 * - Caso contrário, retorna um objeto inválido (`valid = false`).
 * 
 * @param index Índice (linha) do objeto a ser recuperado.
 * @return Estrutura `ObjectStorage` contendo temperatura, umidade e validade.
 */
ObjectStorage getObjectStorage(size_t index) {
  ObjectStorage obj;

  File file = LittleFS.open(PATH, "r");
  if (!file) {
    log(LOG_ERROR, "Erro ao abrir arquivo não existente");
    return obj;
  }

  int cont = 0;
  StaticJsonDocument<128> doc;

  while (file.available() && cont <= index) {
    String line = file.readStringUntil('\n');  // Lê até a quebra de linha
    if (cont == index) {
      DeserializationError err = deserializeJson(doc, line);
      if (!err) {
        obj.temperature = doc["t"] | 0.0;
        obj.humidity    = doc["h"] | 0.0;
        obj.valid       = true;
      } else {
        log(LOG_ERROR, "Erro ao ler linha JSON");
      }
      break;
    }
    cont++;
  }

  file.close();
  return obj;
}

// -----------------------------------------------------------------------------
// Funções de remoção e manutenção
// -----------------------------------------------------------------------------

/**
 * @brief Remove a primeira mensagem armazenada no arquivo.
 * 
 * - Cria um arquivo temporário.
 * - Copia todas as linhas do arquivo original, exceto a primeira.
 * - Substitui o arquivo original pelo temporário.
 */
void deleteOneMessage() {
  File fileIn = LittleFS.open(PATH,"r");
  if (!fileIn) {
    log(LOG_ERROR, "Arquivo não existe");
    return;
  }

  if (fileIn.size() <= 0 ) {
    log(LOG_INFO, "Arquivo vazio");
    fileIn.close();
    hasObj = false;
    return;
  }

  File fileOut = LittleFS.open("/temp.json", "w");
  if (!fileOut) {
    log(LOG_ERROR,"Não foi possível criar arquivo temporário");
    fileIn.close();
    return;
  }

  bool firstLineSkipped = false;

  while (fileIn.available()) {
    String line = fileIn.readStringUntil('\n');
    if (!firstLineSkipped) {
      // Ignora a primeira linha
      firstLineSkipped = true;
      continue;
    }
    // Grava as demais linhas no arquivo temporário
    fileOut.println(line);
  }

  fileIn.close();
  fileOut.close();

  // Substitui o arquivo original pelo novo
  LittleFS.remove(PATH);
  LittleFS.rename("/temp.json", PATH);

  log(LOG_INFO,"objeto removido");
}

/**
 * @brief Verifica se há objetos armazenados.
 * 
 * @return true se existe pelo menos um objeto armazenado.
 * @return false caso contrário.
 */
bool hasDataStorage(){
  return hasObj;
}

/**
 * @brief Imprime no Serial todos os objetos armazenados no arquivo.
 * 
 * - Lê o arquivo linha por linha.
 * - Desserializa cada linha em JSON.
 * - Exibe temperatura e umidade formatadas no console.
 */
void storagePrint() {
  File file = LittleFS.open(PATH, "r");
  if (!file) {
    Serial.println("Arquivo não encontrado");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n'); // Lê até a quebra de linha
    if (line.length() == 0) continue; // Ignora linhas vazias

    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, line);

    if (!err) {
      float t = doc["t"];
      float h = doc["h"];
      Serial.printf("Temp: %.2f, Hum: %.2f\n", t, h);
    } else {
      Serial.println("Erro ao ler linha JSON");
    }
  }

  file.close();
}
