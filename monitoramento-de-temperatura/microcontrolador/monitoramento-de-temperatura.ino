#include "sensor.h"
#include "wifi.h"
#include "mqtt.h"
#include "display_LCD-1602_I2C.h"
#include "buzzer.h"
#include "led.h"
#include "button.h"
#include "log.h"


constexpr unsigned long MQTT_INTERVAL_MS = 180000;  // 3 minutos
constexpr unsigned long SENSOR_INTERVAL_MS = 1000;  // 1 segundo

constexpr uint8_t TEMPERATURE_MAX = 23;
constexpr uint8_t TEMPERATURE_MIN = 14;

constexpr uint8_t HUMIDITY_MIN = 10;
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
    log(LOG_WARN, "Erro na leitura do sensor");
  }

  if (data.temperature > TEMPERATURE_MAX || data.temperature < TEMPERATURE_MIN) {
    status.temperatureError = true;
    log(LOG_WARN, "Temperatura fora do intervalo estipulado");
  }

  if (data.humidity > HUMIDITY_MAX || data.humidity < HUMIDITY_MIN) {
    status.humidityError = true;
    log(LOG_WARN, "Humidade fora do intervalo estipulado");
  }

  return status;
}

void updateDisplay(const EnvironmentData& data, const ErrorStatus& errors) {

  // logSensorData("Display", data);
  log(LOG_INFO, "Dados Display Temperatura: %.2f°C Humidade %.2f%%", data.temperature, data.humidity);
  // Passa os dados e os status de alerta para a função de exibição
  lcd1602_showData(data.temperature, data.humidity, errors.temperatureError, errors.humidityError);
}

void maybePublishMQTT(const EnvironmentData& data, unsigned long now) {

  if (now - lastMQTTPublishTime >= MQTT_INTERVAL_MS) {
    lastMQTTPublishTime = now;
    publishSensorData(data.temperature, data.humidity);

    log(LOG_INFO, "Dados MQTT Temperatura: %.2f°C Humidade %.2f%%", data.temperature, data.humidity);
  }
}

void handleSensorData(EnvironmentData& data, unsigned long now) {

  ErrorStatus errors = checkErrors(data);

  updateDisplay(data, errors);

  if (errors.sensorError) {
    log(LOG_ERROR, "Erro no sensor de temperatura");
    return;
  }

  maybeHandleAlerts(errors, now);
  maybePublishMQTT(data, now);
}

void maybeHandleAlerts(const ErrorStatus& errors, unsigned long now) {
  if (errors.humidityError || errors.temperatureError) {

    enableButtonInterrupt();

    if (buttonWasPressed()) {
      disableSoundAlert();

    } else {
      toggleBuzzer(now);
    }
    toggleLed(now);
  } else {

    disableButtonInterrupt();
    resetButtonState();
    disableSoundAlert();
    turnOffLed();
  }
}


void setup() {

  logInit(LOG_INFO);

  buttonInit();
  log(LOG_INFO, "Botao iniciado");

  initializeSensor();
  log(LOG_INFO, "Sensor iniciado");

  // Chama a função para iniciar o display
  lcd1602_init();
  log(LOG_INFO, "LCD iniciada");

  buzzerInit();
  log(LOG_INFO, "Buzzer iniciado iniciado");

  ledInit();
  log(LOG_INFO, "LED iniciado");


  if (!connectWiFi()) {
    log(LOG_ERROR, "Erro na conexão com wifi");
  }

  setupMQTT();
}

void loop() {

  reconnectWifi();
  client.loop();
  checkMQTTConnected();

  unsigned long now = millis();

  if (now - lastSensorReadTime >= SENSOR_INTERVAL_MS) {
    lastSensorReadTime = now;
    EnvironmentData data = readSensorData();
    handleSensorData(data, now);
  }
}
