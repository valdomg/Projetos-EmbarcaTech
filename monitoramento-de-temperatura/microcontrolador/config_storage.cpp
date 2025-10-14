#include "config_storage.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "log.h"

const char* CONFIG_PATH = "/config.json";

ConfigurationData cfg;


void initConfigStorage() {
  // LittleFS já foi inicializado em outro módulo
  // (mantém a função apenas para consistência)
  log(LOG_INFO, "Config storage pronto (LittleFS já montado)");
}

bool hasConfigData() {
  return LittleFS.exists(CONFIG_PATH);
}

ConfigurationData loadConfig() {

  ConfigurationData config;

  File file = LittleFS.open(CONFIG_PATH, "r");

  if (!file) {
    log(LOG_ERROR, "Arquivo de configuraçao não encontrado");
    return config;
  }

  StaticJsonDocument<768> doc;

  if (deserializeJson(doc, file)) {
    log(LOG_ERROR, "Erro ao ler JSON de configuração");
    file.close();
    return config;
  }

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
  config.valid = true;


  file.close();
  return config;
}


bool saveConfigutionData(const ConfigurationData& config) {

  StaticJsonDocument<768> doc;

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


  File file = LittleFS.open(CONFIG_PATH, "w");

  if (!file) {
    log(LOG_ERROR, "Falha ao abrir arquivo para salva configuraçao");
    return false;
  }

  serializeJsonPretty(doc, file);
  file.close();
  log(LOG_INFO, "Arquivo de configuraçao salvo");
  cfg = loadConfig();
  return true;
}