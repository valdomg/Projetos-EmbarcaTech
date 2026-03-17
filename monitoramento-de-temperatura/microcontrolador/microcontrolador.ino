/**
 * @file microncontrolador.ino
 * @brief Sistema de monitoramento ambiental com ESP8266, MQTT e interface local.
 *
 * @details
 * Este programa realiza o monitoramento contínuo da temperatura e humidade
 * utilizando um sensor ambiental originalmente aht10. Os dados são exibidos em um display LCD,
 * enviados para um servidor MQTT e armazenados localmente em caso de falha
 * na comunicação.
 *
 * O sistema também possui:
 *
 * - Sistema de alerta sonoro (buzzer)
 * - Sistema visual (LED)
 * - Botão para silenciar alertas
 * - Interface Web para configuração
 * - Modo Access Point para configuração inicial
 *
 * Fluxo principal:
 *
 * 1. Inicializa hardware e módulos
 * 2. Conecta ao WiFi e MQTT
 * 3. Realiza leitura periódica do sensor
 * 4. Exibe dados no LCD
 * 5. Verifica erros e gera alertas
 * 6. Publica dados via MQTT
 * 7. Inicia modo configuração se necessário
 *
 * @date 2026
 */

 
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
#include "server.h"
#include "config_storage.h"
#include <ESP8266WebServer.h>


ESP8266WebServer server(80);

constexpr unsigned long MQTT_INTERVAL_MS = 180 * 1000;  // 3 minutos
constexpr unsigned long SENSOR_INTERVAL_MS = 1000;      // 1 segundo

unsigned long lastMQTTPublishTime = 0;
unsigned long lastSensorReadTime = 0;


/**
 * @brief Estrutura que representa o estado de erro do sistema.
 *
 * @details
 * Indica se há falhas na leitura do sensor ou se os valores estão
 * fora dos limites configurados.
 */
struct ErrorStatus {
  bool humidityError;
  bool temperatureError;
  bool sensorError;
};

bool hasError = false;


/**
 * @brief Verifica se os dados do sensor estão dentro dos limites.
 *
 * @param data Dados ambientais lidos do sensor
 *
 * @return ErrorStatus Estrutura contendo o status dos erros detectados
 *
 * @details
 * Essa função verifica:
 *
 * - Validade da leitura
 * - Limites de temperatura
 * - Limites de humidade
 *
 * Caso ocorra erro:
 *
 * - Envia alerta MQTT
 * - Atualiza variável global de erro
 * - Registra log
 */
ErrorStatus checkErrors(const EnvironmentData& data) {
  ErrorStatus status{ false, false, false };
  hasError = false;

  if (!data.valid) {
    status.sensorError = true;
    hasError = true;
    publishAlert(data.temperature, data.humidity);
    log(LOG_WARN, "Erro na leitura do sensor");
    return status;
  }
 
  if (data.temperature > cfg.temperatureMax || data.temperature < cfg.temperatureMin) {
    status.temperatureError = true;
    hasError = true;
    publishAlert(data.temperature, data.humidity);
    log(LOG_WARN, "Temperatura fora do intervalo estipulado");
  }

  if (data.humidity > cfg.humidityMax || data.humidity < cfg.humidityMin) {
    status.humidityError = true;
    hasError = true;
    publishAlert(data.temperature, data.humidity);
    log(LOG_WARN, "Humidade fora do intervalo estipulado");
  }

  return status;
}

/**
 * @brief Atualiza o display LCD com os dados ambientais.
 *
 * @param data Dados do sensor
 * @param errors Status de erro detectado
 */
void updateDisplay(const EnvironmentData& data, const ErrorStatus& errors) {

  log(LOG_INFO, "Dados Display Temperatura: %.2f°C Humidade %.2f%%", data.temperature, data.humidity);
  // Passa os dados e os status de alerta para a função de exibição
  lcd1602_showData(data.temperature, data.humidity, errors.temperatureError, errors.humidityError);
}


/**
 * @brief Publica dados no MQTT em intervalo configurado.
 *
 * @param data Dados ambientais
 * @param now Tempo atual em ms
 *
 * @details
 * Caso a publicação falhe, os dados são armazenados localmente.
 */
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


/**
 * @brief Processa os dados lidos do sensor.
 *
 * @param data Dados ambientais
 * @param now Tempo atual
 *
 * @details
 * Executa:
 *
 * - verificação de erros
 * - atualização display
 * - controle de alertas
 * - envio MQTT
 */
void handleSensorData(EnvironmentData& data, unsigned long now) {

  ErrorStatus errors = checkErrors(data);

  updateDisplay(data, errors);

  maybeHandleAlerts(errors, now);
  maybePublishMQTT(data, now);
}

/**
 * @brief Controla os alertas sonoros e visuais.
 *
 * @param errors Estrutura contendo status de erro
 * @param now Tempo atual
 *
 * @details
 * Ativa:
 *
 * - buzzer
 * - LED
 *
 * Permite silenciar via botão.
 */
void maybeHandleAlerts(const ErrorStatus& errors, unsigned long now) {
  if (errors.humidityError || errors.temperatureError || errors.sensorError) {


    enableButtonInterruptRising(PIN_BUTTON_MUTE);

    if (wasMuted()) {
      disableSoundAlert();

    } else {
      toggleBuzzer(now);
    }
    toggleLed(now);  } else {

    disableButtonInterruptRising(PIN_BUTTON_MUTE);
    resetButtonState();
    resetStateSendAlert();
    disableSoundAlert();
    turnOffLed();
  }
}

/**
 * @brief Inicializa todos os módulos do sistema.
 *
 * @details
 * Inicializa:
 *
 * - sistema de log
 * - botões
 * - sensor
 * - armazenamento
 * - display LCD
 * - buzzer
 * - LED
 * - WiFi
 * - MQTT
 *
 * Também carrega as configurações armazenadas.
 */
void setup() {

  logInit(LOG_INFO);

  buttonInit(PIN_BUTTON_MUTE);
  log(LOG_DEBUG, "Botao iniciado");

  buttonInit(PIN_BUTTON_LONG);

  enableButtonInterruptChange(PIN_BUTTON_LONG);

  initializeSensor();

  initStorage();

  // Chama a função para iniciar o display
  lcd1602_init();
  log(LOG_DEBUG, "LCD iniciada");

  buzzerInit();
  log(LOG_DEBUG, "Buzzer iniciado iniciado");

  ledInit();
  log(LOG_DEBUG, "LED iniciado");

  cfg = loadConfig();

  if (!connectWiFi()) {
    log(LOG_ERROR, "Erro na conexão com wifi");
  }

  setupMQTT();
}

/**
 * @brief Loop principal do sistema.
 *
 * @details
 * Executa continuamente:
 *
 * - controle do backlight do LCD
 * - verificação do botão de configuração
 * - modo Access Point para configuração
 * - reconexão WiFi
 * - reconexão MQTT
 * - leitura periódica do sensor
 * - processamento dos dados
 * - envio MQTT
 *
 * O sistema opera em dois modos:
 *
 * Modo normal:
 * - leitura e monitoramento
 *
 * Modo configuração:
 * - servidor web ativo
 */
void loop() {


  handleBacklightLCD(hasError, wasButtonShortPress());

  if (wasButtonLongPressed() || !cfg.valid) {
    disableSoundAlert();
    turnOnLed();
    createAccessPoint();
    startServer(&server);
    server_handle_loop(&server);

  } else {

    stopServer(&server);

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
}
