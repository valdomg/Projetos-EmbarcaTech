#ifndef JSONDATAPROCESSING_H
#define JSONDATAPROCESSING_H

#include <Arduino.h>

extern void processing_json_MQTT(byte* payload, unsigned int length);

const char* createJsonPayload(char* buffer,size_t  bufferSize, int roomNumber);

#endif