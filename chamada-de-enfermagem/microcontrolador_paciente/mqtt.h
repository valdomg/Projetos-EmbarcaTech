#ifndef MQTT_H
#define MQTT_H
#include <PubSubClient.h>

extern PubSubClient client;

extern const char* TOPIC_PUBLISH;
const char* createJsonPayload();


void setupMQTT();
void connectMQTT();
void callback(char *topic, byte * payload, unsigned int length);

#endif