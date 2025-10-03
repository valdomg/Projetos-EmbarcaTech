#include "sensor.h"
#include "wifi.h"
#include "mqtt.h"
#include "display_LCD-1602_I2C.h"
#include "buzzer.h"
#include "led.h"
#include "button.h"
#include "log.h"
#include "config.h"
#include "storage.h"

constexpr unsigned long MQTT_INTERVAL_MS = 180 * 1000;  // 3 minutos
constexpr unsigned long SENSOR_INTERVAL_MS = 1000;      // 1 segundo

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
    return status;
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

  log(LOG_INFO, "Dados Display Temperatura: %.2f°C Humidade %.2f%%", data.temperature, data.humidity);
  // Passa os dados e os status de alerta para a função de exibição
  lcd1602_showData(data.temperature, data.humidity, errors.temperatureError, errors.humidityError);
}

void maybePublishMQTT(const EnvironmentData& data, unsigned long now) {

  if (now - lastMQTTPublishTime >= MQTT_INTERVAL_MS) {
    lastMQTTPublishTime = now;

    if (!publishSensorData(data.temperature, data.humidity)) {
      saveStorage(data.temperature, data.humidity);
      log(LOG_ERROR, "Dados MQTT Temperatura foram guardados");
    } else {

      log(LOG_INFO, "Dados enviados ao MQTT Temperatura: %.2f°C Humidade %.2f%%", data.temperature, data.humidity);
    }
  }
}

void handleSensorData(EnvironmentData& data, unsigned long now) {

  ErrorStatus errors = checkErrors(data);

  updateDisplay(data, errors);

  maybeHandleAlerts(errors, now);
  maybePublishMQTT(data, now);
}

void maybeHandleAlerts(const ErrorStatus& errors, unsigned long now) {
  if (errors.humidityError || errors.temperatureError || errors.sensorError) {

    enableButtonInterrupt(PIN_BUTTON_MUTE);

    if (wasMuted()) {
      disableSoundAlert();

    } else {
      toggleBuzzer(now);
    }
    toggleLed(now);
  } else {

    disableButtonInterrupt(PIN_BUTTON_MUTE);
    resetButtonState();
    disableSoundAlert();
    turnOffLed();
  }
}


void setup() {

  logInit(LOG_INFO);

  buttonInit(PIN_BUTTON_MUTE);
  log(LOG_DEBUG, "Botao iniciado");

  initializeSensor();

  initStorage();

  // Chama a função para iniciar o display
  lcd1602_init();
  log(LOG_DEBUG, "LCD iniciada");

  buzzerInit();
  log(LOG_DEBUG, "Buzzer iniciado iniciado");

  ledInit();
  log(LOG_DEBUG, "LED iniciado");


  if (!connectWiFi()) {
    log(LOG_ERROR, "Erro na conexão com wifi");
  }

  setupMQTT();
}

void loop() {

  if (reconnectWifi()) {
    if (checkMQTTConnected()) {
      resendMqttData();
    }
  }

  unsigned long now = millis();


  if (now - lastSensorReadTime >= SENSOR_INTERVAL_MS) {
    lastSensorReadTime = now;
    EnvironmentData data = readSensorData();
    handleSensorData(data, now);
  }
}
