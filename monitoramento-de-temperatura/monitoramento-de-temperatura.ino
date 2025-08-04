

// ===== INCLUDES E DEFINIÇÕES =====

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Adafruit_AHTX0.h"
#include <stdio.h>
#include <WiFiClientSecure.h>

// ===== CONFIGURAÇÕES DE REDE =====
#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxx"

// ===== CONFIGURAÇÕES MQTT =====
#define MQTT_SERVER "xxxxxxxxx"
#define MQTT_PASS "xxxxxxxxxx"
#define MQTT_USER "xxxxxxxxx"

// ===== PINOS DO SENSOR =====
#define AHT10_SDA 4
#define AHT10_SDL 5

// ===== OBJETOS GLOBAIS =====
Adafruit_AHTX0 aht;

WiFiClientSecure espClient;
PubSubClient client(espClient);

// ===== STRUCTS =====
struct EnvironmentData {  //Estrutura para desacoplamento do sensor
  float temperature;
  float humidity;
  bool valid;
};


// ===== FUNÇÕES UTILITÁRIAS =====

/**
 * Lê os dados de temperatura e umidade do sensor AHT10.
 * 
 * @return Estrutura contendo temperatura (°C), umidade (%) e status de validade.
 */
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

/**
 * Exibe os dados do sensor no monitor serial.
 *
 *@param data - Estrutura contendo os valores de temperatura (°C) e umidade (%).
 */
void printSensorData(const EnvironmentData& data) {

  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.println(" °C");

  Serial.print("Umidade: ");
  Serial.print(data.humidity);
  Serial.println(" %");
}

/**
 * Conecta com WiFi e retorna valor true para conexão bem sucedida.
 */
bool connectWiFi() {

  Serial.print("Conectando com Wi-Fi: ");
  Serial.print(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + 10000;  // 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() < timeout) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(".");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado ao WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("Conexão com WiFi não estabelecida");
    return false;
  }
}

/**
 * Conecta o ESP com o servidor MQTT.
 */
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("conectado");

    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

/**
 * Verifica a conexão com MQTT, e reconecta se necessario.
 */
void checkMQTTConnected() {
  if (!client.connected()) {
    connectMQTT();
  }
}

/**
 * Realiza a leitura do sensor e publica os dados via MQTT.
 */
void publishSensorData() {
  EnvironmentData sensorReading = readSensorData();
  if (!sensorReading.valid) return;
  printSensorData(sensorReading);
  publishFloat("laboratorio/temperatura", sensorReading.temperature);
  publishFloat("laboratorio/humidade", sensorReading.humidity);
}


/**
 * Publica um valor float formatado para duas casas decimais em um tópico MQTT.
 * 
 * @param topic - Tópico MQTT para publicação.
 * @param value - Valor numérico a ser enviado.
 */
void publishFloat(const char* topic, float value) {
  char payload[10];
  snprintf(payload, sizeof(payload), "%.2f", value);
  client.publish(topic, payload);
}


// ===== FUNÇÕES DE INICIALIZAÇÃO =====

/**
 * Inicializa o sensor AHT10.
 */
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

  initializeSensor();

  if (!connectWiFi()) {
    Serial.println("ERRO: Não foi possivel conectar ao wifi");
  }
  espClient.setInsecure();
  client.setServer(MQTT_SERVER, 8883);
}


// ===== FUNÇÃO LOOP PRINCIPAL =====
void loop() {

  checkMQTTConnected();
  publishSensorData();
  client.loop();
  delay(3000);
}
