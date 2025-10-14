#include "config_storage.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "log.h"

// Caminho fixo do arquivo JSON de configuração armazenado no sistema de arquivos interno (LittleFS)
const char* CONFIG_PATH = "/config.json";

// Estrutura global que mantém os dados atuais de configuração do sistema
ConfigurationData cfg;

/**
 * @brief Inicializa o subsistema de armazenamento de configuração.
 * 
 * Essa função é responsável por preparar o módulo de configuração.
 * No caso, o LittleFS já é montado em outro módulo, portanto aqui
 * apenas é registrada uma mensagem de log para indicar que o sistema
 * de configuração está pronto para uso.
 */
void initConfigStorage() {
  log(LOG_INFO, "Config storage pronto (LittleFS já montado)");
}

/**
 * @brief Verifica se existe um arquivo de configuração salvo.
 * 
 * @return true se o arquivo /config.json existir.
 * @return false caso o arquivo não exista.
 */
bool hasConfigData() {
  return LittleFS.exists(CONFIG_PATH);
}

/**
 * @brief Carrega os dados de configuração do arquivo JSON.
 * 
 * Essa função abre o arquivo `/config.json`, lê seu conteúdo e
 * converte os valores armazenados em uma estrutura `ConfigurationData`.
 * Caso ocorra erro na leitura ou o arquivo não exista, retorna uma
 * estrutura vazia (com `valid = false`).
 * 
 * @return Estrutura ConfigurationData contendo os valores carregados.
 */
ConfigurationData loadConfig() {

  ConfigurationData config;

  File file = LittleFS.open(CONFIG_PATH, "r");

  if (!file) {
    log(LOG_ERROR, "Arquivo de configuração não encontrado");
    return config;
  }

  // Documento JSON com tamanho máximo estimado de 768 bytes
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
  config.mqttTopicData = doc["MQTT_TOPIC_PUBLICATION_DATA"].as<String>();
  config.mqttTopicAlert = doc["MQTT_TOPIC_PUBLICATION_ALERT"].as<String>();
  config.mqttDeviceId = doc["MQTT_DEVICE_ID"].as<String>();
  config.wifiPass = doc["WIFI_PASS"].as<String>();
  config.wifiSSID = doc["WIFI_SSID"].as<String>();
  config.temperatureMax = doc["TEMPERATURE_MAX"].as<float>();
  config.temperatureMin = doc["TEMPERATURE_MIN"].as<float>();
  config.humidityMax = doc["HUMIDITY_MAX"].as<float>();
  config.humidityMin = doc["HUMIDITY_MIN"].as<float>();
  config.valid = true;  // Indica que a configuração foi carregada com sucesso

  file.close();
  return config;
}

/**
 * @brief Salva os dados de configuração no arquivo JSON.
 * 
 * Essa função cria ou sobrescreve o arquivo `/config.json` no
 * sistema de arquivos LittleFS, armazenando todos os valores da
 * estrutura `ConfigurationData` em formato JSON.
 * 
 * Após salvar, a função recarrega os dados salvos em `cfg` para
 * garantir consistência e retorna um valor booleano indicando sucesso.
 * 
 * @param config Estrutura ConfigurationData contendo os dados a serem salvos.
 * @return true se o arquivo for salvo corretamente.
 * @return false se ocorrer erro ao abrir ou escrever o arquivo.
 */
bool saveConfigutionData(const ConfigurationData& config) {

  StaticJsonDocument<768> doc;

  // Converte a estrutura de configuração para JSON
  doc["MQTT_SERVER"] = config.mqttServer;
  doc["MQTT_USER"] = config.mqttUser;
  doc["MQTT_PASS"] = config.mqttPass;
  doc["MQTT_TOPIC_PUBLICATION_DATA"] = config.mqttTopicData;
  doc["MQTT_TOPIC_PUBLICATION_ALERT"] = config.mqttTopicAlert;
  doc["MQTT_DEVICE_ID"] = config.mqttDeviceId;
  doc["WIFI_PASS"] = config.wifiPass;
  doc["WIFI_SSID"] = config.wifiSSID;
  doc["TEMPERATURE_MAX"] = config.temperatureMax;
  doc["TEMPERATURE_MIN"] = config.temperatureMin;
  doc["HUMIDITY_MAX"] = config.humidityMax;
  doc["HUMIDITY_MIN"] = config.humidityMin;

  // Abre o arquivo em modo escrita (cria ou sobrescreve)
  File file = LittleFS.open(CONFIG_PATH, "w");

  if (!file) {
    log(LOG_ERROR, "Falha ao abrir arquivo para salvar configuração");
    return false;
  }

  // Salva o conteúdo JSON no arquivo de forma formatada
  serializeJsonPretty(doc, file);
  file.close();

  log(LOG_INFO, "Arquivo de configuração salvo");

  // Recarrega a configuração salva para manter a variável global atualizada
  cfg = loadConfig();

  return true;
}
