#ifndef JSONDATAPROCESSING_H
#define JSONDATAPROCESSING_H

#include <Arduino.h>

extern bool processing_json_MQTT(byte* payload, unsigned int length);

const char* createJsonPayload(char* buffer, size_t length, const char* roomNumber);

const char* getPayloadID(byte* payload, unsigned int length);

const char* creteJsonPayloadConfirmationMessage(char* buffer, size_t length);

#endif