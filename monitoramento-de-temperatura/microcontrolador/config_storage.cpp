/**
 * @file config_storage.cpp
 * @brief Implementação do subsistema de armazenamento de configuração.
 *
 * @details
 * Este módulo é responsável por persistir e recuperar as configurações
 * do sistema utilizando o sistema de arquivos LittleFS em formato JSON.
 *
 * As configurações incluem:
 *
 * - Parâmetros de conexão Wi-Fi
 * - Configuração do broker MQTT
 * - Tópicos de publicação
 * - Limites de temperatura e umidade
 *
 * O armazenamento é realizado no arquivo:
 *
 * `/config.json`
 *
 * Este módulo garante:
 *
 * - Persistência entre reinicializações
 * - Carregamento seguro
 * - Validação básica
 *
 * @note O sistema de arquivos LittleFS deve ser montado antes da utilização
 *       deste módulo.
 */

#include "config_storage.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "log.h"


// ============================================================================
// Constantes
// ============================================================================

/**
 * @brief Caminho do arquivo de configuração no LittleFS.
 *
 * Este arquivo armazena todas as configurações do sistema em formato JSON.
 */
const char* CONFIG_PATH = "/config.json";

// ============================================================================
// Variáveis globais
// ============================================================================

/**
 * @brief Estrutura global contendo a configuração ativa do sistema.
 *
 * Esta variável é atualizada após o carregamento ou salvamento
 * das configurações.
 *
 * @warning Deve ser acessada somente após loadConfig() ou saveConfigutionData().
 */
ConfigurationData cfg;

// ============================================================================
// Inicialização
// ============================================================================

/**
 * @brief Inicializa o subsistema de armazenamento de configuração.
 *
 * @details
 * Esta função prepara o módulo de armazenamento de configuração.
 *
 * Atualmente, assume que o LittleFS já foi montado por outro módulo (storage.h)
 *
 * Apenas registra uma mensagem de log indicando que o sistema está pronto.
 *
 * @note Esta função não monta o LittleFS.
 *
 * @see LittleFS.begin()
 */
void initConfigStorage() {
  log(LOG_INFO, "Config storage pronto (LittleFS já montado)");
}

// ============================================================================
// Verificação de existência
// ============================================================================

/**
 * @brief Verifica se existe um arquivo de configuração salvo.
 *
 * @details
 * Essa função verifica a existência do arquivo `/config.json`
 * no sistema de arquivos LittleFS.
 *
 * @retval true  Arquivo existe.
 * @retval false Arquivo não existe.
 */
bool hasConfigData() {
  return LittleFS.exists(CONFIG_PATH);
}

// ============================================================================
// Carregamento
// ============================================================================

/**
 * @brief Carrega as configurações do sistema a partir do arquivo.
 *
 * @details
 * Esta função realiza as seguintes etapas:
 *
 * 1. Abre o arquivo `/config.json`
 * 2. Lê o conteúdo JSON
 * 3. Converte os dados para a estrutura ConfigurationData
 * 4. Retorna a estrutura preenchida
 *
 * Em caso de erro:
 *
 * - Arquivo inexistente
 * - Erro de leitura
 * - JSON inválido
 *
 * retorna uma estrutura com:
 *
 * valid = false
 *
 * @return ConfigurationData Estrutura contendo as configurações carregadas.
 *
 * @note O tamanho do JSON é limitado a 768 bytes.
 *
 * @warning Se o JSON estiver corrompido, os dados serão descartados.
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
  config.mqttPort = doc["MQTT_PORT"].as<int>();
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

// ============================================================================
// Salvamento
// ============================================================================

/**
 * @brief Salva a configuração no sistema de arquivos.
 *
 * @details
 * Esta função:
 *
 * - Cria ou sobrescreve o arquivo `/config.json`
 * - Converte a estrutura ConfigurationData para JSON
 * - Salva no LittleFS
 * - Atualiza a variável global cfg
 *
 * @param config Configuração a ser salva.
 *
 * @retval true  Configuração salva com sucesso.
 * @retval false Falha ao salvar.
 *
 * @note Após salvar, a configuração é recarregada automaticamente.
 *
 * @warning A configuração anterior será sobrescrita.
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
  doc["MQTT_PORT"] = config.mqttPort;
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
