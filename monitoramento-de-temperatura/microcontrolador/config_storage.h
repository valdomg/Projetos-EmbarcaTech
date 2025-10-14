#ifndef CONFIG_STORAGE
#define CONFIG_STORAGE

#include <Arduino.h>


struct ConfigurationData {
  String mqttServer;
  String mqttUser;
  String mqttPass;
  String mqttTopicData;
  String mqttTopicAlert;
  String mqttDeviceId;
  String wifiPass;
  String wifiSSID;
  float temperatureMax;
  float temperatureMin;
  float humidityMax;
  float humidityMin;
  bool valid = false; 
};


void initConfigStorage();
ConfigurationData loadConfig();
bool saveConfigutionData(const ConfigurationData &config);
bool hasConfigData();
extern ConfigurationData cfg;

#endif 