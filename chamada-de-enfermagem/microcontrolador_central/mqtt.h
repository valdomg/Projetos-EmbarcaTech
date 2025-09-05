#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

#include <PubSubClient.h>

extern PubSubClient client;

void setupMQTT();
void checkMQTTConnected();

void callback(char* topic, byte* payload, unsigned int length);
void publicReponseDivice(const char* id, float value);

#endif
