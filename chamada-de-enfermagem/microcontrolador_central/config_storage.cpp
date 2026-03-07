/**
 * @file config_storage.cpp
 * @brief Implementação do módulo de armazenamento de configurações do sistema.
 *
 * @details
 * Este módulo é responsável por gerenciar o armazenamento persistente
 * das configurações do dispositivo utilizando o sistema de arquivos
 * interno LittleFS.
 *
 * As configurações são armazenadas no arquivo `/config.json`
 * em formato JSON e incluem parâmetros necessários para o
 * funcionamento do sistema, como:
 *
 * - Credenciais da rede WiFi
 * - Configurações do servidor MQTT
 * - Identificação do dispositivo
 *
 * O módulo fornece funcionalidades para:
 *
 * - Inicializar o subsistema de armazenamento
 * - Verificar se existe configuração salva
 * - Carregar configurações do arquivo
 * - Salvar novas configurações
 *
 * A biblioteca ArduinoJson é utilizada para realizar a
 * serialização e desserialização dos dados em formato JSON.
 *
 * O uso do LittleFS permite que as configurações permaneçam
 * armazenadas mesmo após reinicializações do dispositivo.
 *
 * @note Este módulo depende do sistema de arquivos LittleFS.
 *
 * @date 2026
 */

#include "config_storage.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "log.h"

/**
 * @brief Caminho do arquivo de configuração no sistema de arquivos.
 *
 * @details
 * Define o local onde os dados de configuração do dispositivo
 * são armazenados no sistema de arquivos interno (LittleFS).
 *
 * O arquivo utiliza o formato JSON para armazenar parâmetros
 * como credenciais WiFi e configurações do servidor MQTT.
 */
const char* CONFIG_PATH = "/config.json";


/**
 * @brief Estrutura global contendo a configuração atual do sistema.
 *
 * @details
 * Essa variável mantém os dados de configuração carregados do
 * arquivo `/config.json`.
 *
 * Ela é atualizada quando:
 *
 * - O sistema carrega a configuração do armazenamento
 * - Uma nova configuração é salva no dispositivo
 *
 * A estrutura `ConfigurationData` contém parâmetros como:
 *
 * - Credenciais WiFi
 * - Configurações do broker MQTT
 * - Identificação do dispositivo
 */
ConfigurationData cfg;


/**
 * @brief Inicializa o subsistema de armazenamento de configuração.
 *
 * @details
 * Essa função inicializa o sistema de arquivos LittleFS,
 * responsável por armazenar o arquivo de configuração do
 * dispositivo.
 *
 * Caso o sistema de arquivos não consiga ser montado,
 * uma mensagem de erro é registrada no sistema de log.
 *
 * @note Deve ser chamada durante a inicialização do sistema
 * (normalmente no `setup()`).
 */
void initConfigStorage() {

  if (!LittleFS.begin()) {
    log(LOG_ERROR, "Erro ao montar LittleFS");
    return;
  }

}


/**
 * @brief Verifica se existe um arquivo de configuração salvo.
 *
 * @details
 * Essa função verifica se o arquivo `/config.json` está
 * presente no sistema de arquivos interno.
 *
 * @return true  Se o arquivo de configuração existir.
 * @return false Caso o arquivo não exista.
 */
bool hasConfigData() {
  return LittleFS.exists(CONFIG_PATH);
}


/**
 * @brief Carrega os dados de configuração do arquivo JSON.
 *
 * @details
 * Abre o arquivo `/config.json`, lê seu conteúdo e converte
 * os valores armazenados para a estrutura `ConfigurationData`.
 *
 * O processo consiste em:
 *
 * 1. Abrir o arquivo de configuração.
 * 2. Desserializar o conteúdo JSON.
 * 3. Mapear os campos JSON para a estrutura de configuração.
 * 4. Retornar a estrutura preenchida.
 *
 * Caso ocorra erro em qualquer etapa, uma estrutura
 * inválida é retornada (`valid = false`).
 *
 * @return Estrutura `ConfigurationData` contendo os dados carregados.
 */
ConfigurationData loadConfig() {

  ConfigurationData config;

  File file = LittleFS.open(CONFIG_PATH, "r");

  if (!file) {
    log(LOG_ERROR, "Arquivo de configuração não encontrado");
    return config;
  }

  /**
   * Documento JSON utilizado para leitura da configuração.
   *
   * O tamanho foi definido como 768 bytes para acomodar
   * todos os campos esperados no arquivo de configuração.
   */
  StaticJsonDocument<768> doc;

  // Tenta desserializar o JSON do arquivo
  if (deserializeJson(doc, file)) {
    log(LOG_ERROR, "Erro ao ler JSON de configuração");
    file.close();
    return config;
  }

  // Mapeia os campos do JSON para a estrutura ConfigurationData
  config.mqttServer = doc["MQTT_SERVER"].as<String>();
  config.mqttUser = doc["MQTT_USER"].as<String>();
  config.mqttPass = doc["MQTT_PASS"].as<String>();
  config.mqttDeviceId = doc["MQTT_DEVICE_ID"].as<String>();
  config.mqttPort = doc["MQTT_PORT"].as<int>();
  config.wifiPass = doc["WIFI_PASS"].as<String>();
  config.wifiSSID = doc["WIFI_SSID"].as<String>();

  /**
   * Indica que os dados foram carregados corretamente.
   */
  config.valid = true;

  file.close();

  return config;
}


/**
 * @brief Salva os dados de configuração no arquivo JSON.
 *
 * @details
 * Essa função cria ou sobrescreve o arquivo `/config.json`
 * no sistema de arquivos LittleFS, armazenando os dados
 * contidos na estrutura `ConfigurationData`.
 *
 * O processo consiste em:
 *
 * 1. Converter a estrutura de configuração para JSON.
 * 2. Abrir o arquivo em modo de escrita.
 * 3. Serializar o JSON no arquivo.
 * 4. Recarregar a configuração salva na variável global `cfg`.
 *
 * O uso de `serializeJsonPretty()` facilita a leitura
 * manual do arquivo durante depuração.
 *
 * @param config Estrutura contendo os dados de configuração.
 *
 * @return true  Se os dados foram salvos corretamente.
 * @return false Caso ocorra erro ao abrir ou escrever o arquivo.
 */
bool saveConfigutionData(const ConfigurationData& config) {

  StaticJsonDocument<768> doc;

  // Converte a estrutura de configuração para JSON
  doc["MQTT_SERVER"] = config.mqttServer;
  doc["MQTT_USER"] = config.mqttUser;
  doc["MQTT_PASS"] = config.mqttPass;
  doc["MQTT_DEVICE_ID"] = config.mqttDeviceId;
  doc["MQTT_PORT"] = config.mqttPort;
  doc["WIFI_PASS"] = config.wifiPass;
  doc["WIFI_SSID"] = config.wifiSSID;

  /**
   * Abre o arquivo de configuração em modo escrita.
   *
   * Caso o arquivo já exista, ele será sobrescrito.
   */
  File file = LittleFS.open(CONFIG_PATH, "w");

  if (!file) {
    log(LOG_ERROR, "Falha ao abrir arquivo para salvar configuração");
    return false;
  }

  // Serializa o JSON no arquivo
  serializeJsonPretty(doc, file);

  file.close();

  log(LOG_INFO, "Arquivo de configuração salvo");

  /**
   * Recarrega a configuração para manter a variável
   * global `cfg` sincronizada com o conteúdo salvo.
   */
  cfg = loadConfig();

  return true;
}