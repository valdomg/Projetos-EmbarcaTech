#include "wifi.h"


void setup() {

    Serial.begin(115200);
    connectWiFi();

}

void loop() {
    
    checkConnection();

}