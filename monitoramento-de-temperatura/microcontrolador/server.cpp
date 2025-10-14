#include "server.h"
#include "Arduino.h"
#include "log.h"
#include <ESP8266WiFi.h>
#include "config_storage.h"

bool serverRunning = false;


void serverSetupRoutes(ESP8266WebServer* server) {

  server->on("/", [server]() {
    String html = "<h1>Configuracao do Sistema</h1>";
    html += "<form action='/submit' method='POST'>";

    // --- Config Wi-Fi ---
    html += "<h3>Configuracao Wi-Fi</h3>";
    html += "SSID: <input name='wifiSSID' required><br>";
    html += "Senha: <input name='wifiPass' type='password' required><br><br>";

    // --- Config MQTT ---
    html += "<h3>Configuração MQTT</h3>";
    html += "Servidor MQTT: <input name='mqttServer' required><br>";
    html += "Usuario: <input name='mqttUser' required><br>";
    html += "Senha: <input name='mqttPass' type='password' required><br>";
    html += "Topico de Dados: <input name='mqttTopicData' required><br>";
    html += "Topico de Alerta: <input name='mqttTopicAlert' required><br>";
    html += "Device ID: <input name='mqttDeviceId' required><br><br>";

    // --- Limites de sensores ---
    html += "<h3>Limites de Temperatura e Umidade</h3>";
    html += "Temperatura Maxima: <input name='temperatureMax' type='number' step='0.1' required><br>";
    html += "Temperatura Minima: <input name='temperatureMin' type='number' step='0.1' required><br>";
    html += "Umidade Maxima: <input name='humidityMax' type='number' step='0.1' required><br>";
    html += "Umidade Minima: <input name='humidityMin' type='number' step='0.1' required><br><br>";

    html += "<input type='submit' value='Salvar Configurações'>";
    html += "</form>";
    server->send(200, "text/html", html);
  });

  server->on("/submit", [server]() {


     // --- Wi-Fi ---
  cfg.wifiSSID = server->arg("wifiSSID");
  cfg.wifiPass = server->arg("wifiPass");

  // --- MQTT ---
  cfg.mqttServer = server->arg("mqttServer");
  cfg.mqttUser = server->arg("mqttUser");
  cfg.mqttPass = server->arg("mqttPass");
  cfg.mqttTopicData = server->arg("mqttTopicData");
  cfg.mqttTopicAlert = server->arg("mqttTopicAlert");
  cfg.mqttDeviceId = server->arg("mqttDeviceId");

  // --- Limites ---
  cfg.temperatureMax = server->arg("temperatureMax").toFloat();
  cfg.temperatureMin = server->arg("temperatureMin").toFloat();
  cfg.humidityMax = server->arg("humidityMax").toFloat();
  cfg.humidityMin = server->arg("humidityMin").toFloat();

  saveConfigutionData(cfg);
  ESP.restart();
  });
}


void startServer(ESP8266WebServer* server) {

  if (serverRunning) return;

  serverSetupRoutes(server);
  server->begin();
  serverRunning = true;
}

void stopServer(ESP8266WebServer* server) {

  if (!serverRunning) return;

  // encerra cliente atual (se houver)
  if (server->client()) server->client().stop();

  // para o servidor HTTP
  server->stop();
  serverRunning = false;
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.println("Servidor parado");
}

void server_handle_loop(ESP8266WebServer* server) {
  server->handleClient();
}
