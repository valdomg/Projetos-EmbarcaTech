// Código para leitura de temperatura e umidade usando o sensor AHT10 com ESP8266 D1 MINI
// Os dados são exibidos no monitor serial a cada 3 segundos

// ===== INCLUDES E DEFINIÇÕES =====
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "Adafruit_AHTX0.h"
#include "display_LCD-1602_I2C.h"

#define SSID "XXXXXXX"
#define PASSWORD "XXXXXXXX"

#define AHT10_SDA 4
#define AHT10_SDL 5

// ===== OBJETOS GLOBAIS =====
Adafruit_AHTX0 aht;


// ===== STRUCTS =====
struct EnvironmentData {  //Estrutura para desacoplamento do sensor
  float temperature;
  float humidity;
  bool valid;
};


// ===== FUNÇÕES UTILITÁRIAS =====
EnvironmentData readSensorData() {
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  EnvironmentData data;
  if (isnan(temp.temperature) || isnan(hum.relative_humidity)) {
    data.valid = false;
    return data;
  }

  data.temperature = temp.temperature;
  data.humidity = hum.relative_humidity;
  data.valid = true;

  return data;
}

void printSensorData(const EnvironmentData& data) {

  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(data.humidity);
  Serial.println(" %");
}

// Chama a função de mostrar os dados de temperatura na tela
void printDataDisplay(const EnvironmentData& data) {
  // Passa os dados e os status de alerta para a função de exibição
  show_data_tempHum_DisplayLCD_1602_I2C(data.temperature, data.humidity, false, false);
}

bool conectWiFi() {

  Serial.print("Conectando com Wi-Fi: ");
  Serial.print(SSID);

  WiFi.begin(SSID, PASSWORD);

  unsigned long timeout = millis() + 10000;  // 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(".");

  if (WiFi.status() == WL_CONNECTED){
    Serial.print("Conectado ao WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("Conexão com WiFi não estabelecida");
    return false;
  }

}


// ===== FUNÇÕES DE INICIALIZAÇÃO =====
void initializeSensor() {

  Wire.begin(AHT10_SDA, AHT10_SDL);
  Serial.println("Iniciando sensor AHT10 ...");

  if (!aht.begin()) {
    Serial.println("Sensor AHT10 não inicializado");
    while (1) delay(10);
  }

  Serial.println("Sensor AHT10 inicializado com sucesso!");
}


// ===== FUNÇÃO SETUP =====
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  initializeSensor();
  // Chama a função para iniciar o display
  initializeDisplayLCD_1602_I2C();

  if(!conectWiFi()){

    Serial.println("ERRO: Não foi possivel conectar ao wifi");
  }
  
}


// ===== FUNÇÃO LOOP PRINCIPAL =====
void loop() {
  EnvironmentData sensorReading = readSensorData();
  if (sensorReading.valid) {
    printSensorData(sensorReading);
    // Exibir dados no display
    printDataDisplay(sensorReading);
  }
  delay(1000);
}
