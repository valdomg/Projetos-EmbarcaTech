#include "server.h"
#include "Arduino.h"
#include "log.h"
#include <ESP8266WiFi.h>

bool serverRunning = false;


void serverSetupRoutes(ESP8266WebServer* server) {

  server->on("/", [server]() {
    String html = "<h1>Servidor no modo Access Point</h1>";
    html += "<p>Você está conectado ao ESP!</p>";
    html += "<form action='/submit' method='POST'>";
    html += "Digite algo: <input name='inputText'><br>";
    html += "<input type='submit' value='Enviar'>";
    html += "</form>";
    server->send(200, "text/html", html);
  });

  server->on("/submit", [server]() {
    String input = "";
    if (server->hasArg("inputText")) input = server->arg("inputText");

    String response = "Você enviou: " + input;
    server->send(200, "text/html", response);

    Serial.println("Input recebido: " + input);
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
