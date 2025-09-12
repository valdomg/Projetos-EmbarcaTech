#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

extern PubSubClient client;

void setupMQTT();
void checkMQTTConnected();
void publishSensorData(float temperature, float humidity);
void publishAlert(const char* alert);

#endif
