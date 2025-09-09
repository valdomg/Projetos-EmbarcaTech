#include "sensor.h"
#include "wifi.h"
#include "mqtt.h"
#include "display_LCD-1602_I2C.h"
#include "buzzer.h"
#include "led.h"


constexpr unsigned long MQTT_INTERVAL_MS = 180000;  // 3 minutos
constexpr unsigned long SENSOR_INTERVAL_MS = 1000;  // 1 segundo

constexpr uint8_t TEMPERATURE_MAX = 23;
constexpr uint8_t TEMPERATURE_MIN = 14;

constexpr uint8_t HUMIDITY_MIN = 30;
constexpr uint8_t HUMIDITY_MAX = 60;

unsigned long lastMQTTPublishTime = 0;
unsigned long lastSensorReadTime = 0;

struct ErrorStatus {
  bool humidityError;
  bool temperatureError;
  bool sensorError;
};

ErrorStatus
checkErrors(const EnvironmentData& data) {
  ErrorStatus status{ false, false, false };

  if (!data.valid) {
    status.sensorError = true;
  }

  if (data.temperature > TEMPERATURE_MAX || data.temperature < TEMPERATURE_MIN) {
    status.temperatureError = true;
  }

  if (data.humidity > HUMIDITY_MAX || data.humidity < HUMIDITY_MIN) {
    status.humidityError = true;
  }

  return status;
}

void logSensorData(const char* context, const EnvironmentData& data) {

  Serial.printf("==== valor mostrado no %s ====\n", context);
  printSensorData(data);
  Serial.println("===================================\n");
}

void updateDisplay(const EnvironmentData& data, const ErrorStatus& errors) {

  logSensorData("Display", data);
  // Passa os dados e os status de alerta para a função de exibição
  lcd1602_showData(data.temperature, data.humidity, errors.temperatureError, errors.humidityError);
}

void maybePublishMQTT(const EnvironmentData& data, unsigned long now) {

  if (now - lastMQTTPublishTime >= MQTT_INTERVAL_MS) {
    lastMQTTPublishTime = now;
    publishSensorData(data.temperature, data.humidity);

    logSensorData("MQTT", data);
  }
}

void handleSensorData(EnvironmentData& data, unsigned long now) {

  ErrorStatus errors = checkErrors(data);

  updateDisplay(data, errors);

  if (errors.sensorError) {
    Serial.println("Sensor falhou");
    return;
  }

  maybeHandleAlerts(errors, now);
  maybePublishMQTT(data, now);
}

void maybeHandleAlerts(const ErrorStatus& errors, unsigned long now) {
  if (errors.humidityError || errors.temperatureError) {
    toggleBuzzer(now);
    toggleLed(now);
  } else {
    disableSoundAlert();
    turnOffLed();
  }
}


void setup() {
  Serial.begin(115200);
  initializeSensor();
  // Chama a função para iniciar o display
  lcd1602_init();

  buzzerInit();
  ledInit();

  if (!connectWiFi()) {
    Serial.println("WiFi não conectado.");
  }
  setupMQTT();
}

void loop() {
  client.loop();
  checkMQTTConnected();

  unsigned long now = millis();
  if (now - lastSensorReadTime >= SENSOR_INTERVAL_MS) {
    lastSensorReadTime = now;
    EnvironmentData data = readSensorData();
    handleSensorData(data, now);
  }
}
