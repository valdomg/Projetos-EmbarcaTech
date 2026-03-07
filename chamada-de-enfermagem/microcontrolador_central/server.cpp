/**
 * @file server.cpp
 * @brief Implementação do servidor HTTP para configuração do dispositivo.
 *
 * @details
 * Este módulo implementa um servidor HTTP embarcado utilizado para
 * configuração inicial do sistema através de uma interface web.
 *
 * O servidor permite que o usuário configure parâmetros essenciais
 * do dispositivo diretamente pelo navegador, sem necessidade de
 * reprogramar o firmware.
 *
 * As principais funcionalidades incluem:
 *
 * - Exibir uma página web de configuração do sistema
 * - Receber parâmetros de configuração via formulário HTML
 * - Armazenar configurações em memória persistente
 * - Reiniciar o dispositivo após salvar novas configurações
 *
 * ## Parâmetros configuráveis
 *
 * A interface web permite configurar:
 *
 * ### Wi-Fi
 * - SSID da rede
 * - Senha da rede
 *
 * ### MQTT
 * - Endereço do broker MQTT
 * - Usuário
 * - Senha
 * - Device ID do dispositivo
 * - Porta do broker
 *
 * ## Fluxo de funcionamento
 *
 * 1. O dispositivo inicia em modo **Access Point**.
 * 2. O usuário conecta-se ao AP e acessa a página web do dispositivo.
 * 3. O servidor exibe um formulário de configuração.
 * 4. O usuário envia os parâmetros através do formulário.
 * 5. O sistema salva as configurações na memória persistente.
 * 6. O dispositivo reinicia para aplicar as novas configurações.
 *
 * ## Dependências
 *
 * Este módulo utiliza:
 *
 * - ESP8266WebServer (servidor HTTP)
 * - ESP8266WiFi (controle da interface Wi-Fi)
 * - config_storage (armazenamento das configurações)
 *
 * ## Arquitetura
 *
 * As principais funções deste módulo são:
 *
 * - startServer() → inicia o servidor HTTP
 * - stopServer() → encerra o servidor
 * - server_handle_loop() → processa requisições HTTP
 * - serverSetupRoutes() → define os endpoints da aplicação
 *
 * @note
 * O servidor é utilizado apenas para configuração do dispositivo
 * e normalmente é executado quando o sistema não possui configurações
 * válidas ou quando entra em modo de configuração.
 */

#include "server.h"
#include "Arduino.h"
#include "log.h"
#include <ESP8266WiFi.h>
#include "config_storage.h"

// -----------------------------------------------------------------------------
// Variáveis globais
// -----------------------------------------------------------------------------

/**
 * @brief Indica se o servidor HTTP está em execução.
 *
 * Esta flag evita que o servidor seja iniciado múltiplas vezes.
 * É configurada para `true` após a inicialização bem-sucedida
 * e retornada a `false` quando o servidor é parado.
 */
bool serverRunning = false;


// -----------------------------------------------------------------------------
// Definição das rotas HTTP
// -----------------------------------------------------------------------------

/**
 * @brief Configura as rotas e endpoints do servidor web.
 *
 * Esta função registra os caminhos (URLs) que o servidor HTTP irá tratar:
 *
 * - **"/"**: Exibe uma página HTML de configuração do sistema,
 *   permitindo ao usuário definir parâmetros de Wi-Fi, MQTT e limites ambientais.
 *
 * - **"/submit"**: Recebe os dados enviados pelo formulário e
 *   salva as configurações na memória persistente.
 *
 * @param server Ponteiro para a instância do servidor HTTP (`ESP8266WebServer`).
 */
void serverSetupRoutes(ESP8266WebServer* server) {

  // --- Página principal: formulário de configuração ---
  server->on("/", [server]() {
    String html = "<h1>Configuracao do Sistema</h1>";
    html += "<form action='/submit' method='POST'>";

    // --- Configuração de Wi-Fi ---
    html += "<h3>Configuracao Wi-Fi</h3>";
    html += "SSID: <input name='wifiSSID' required><br>";
    html += "Senha: <input name='wifiPass' type='password' required><br><br>";

    // --- Configuração de MQTT ---
    html += "<h3>Configuracao MQTT</h3>";
    html += "Servidor MQTT: <input name='mqttServer' required><br>";
    html += "Usuario: <input name='mqttUser' required><br>";
    html += "Senha: <input name='mqttPass' type='password' required><br>";
    html += "Device ID: <input name='mqttDeviceId' required><br>";
    html += "Porta: <input name='mqttPort' type='number' required><br><br>";

    html += "<input type='submit' value='Salvar Configurações'>";
    html += "</form>";

    server->send(200, "text/html", html);
  });

  // --- Endpoint de submissão do formulário ---
  server->on("/submit", [server]() {

    // --- Wi-Fi ---
    cfg.wifiSSID = server->arg("wifiSSID");
    cfg.wifiPass = server->arg("wifiPass");

    // --- MQTT ---
    cfg.mqttServer = server->arg("mqttServer");
    cfg.mqttUser = server->arg("mqttUser");
    cfg.mqttPass = server->arg("mqttPass");
    cfg.mqttDeviceId = server->arg("mqttDeviceId");
    cfg.mqttPort = server->arg("mqttPort").toInt();

    // --- Salva as configurações e reinicia o sistema ---
    saveConfigutionData(cfg);
    ESP.restart(); // reseta o esp
  });
}


// -----------------------------------------------------------------------------
// Controle do servidor
// -----------------------------------------------------------------------------

/**
 * @brief Inicia o servidor HTTP e registra suas rotas.
 *
 * Esta função garante que o servidor só será iniciado uma vez.
 * Chama `serverSetupRoutes()` para registrar os endpoints e
 * inicia o servidor com `server->begin()`.
 *
 * @param server Ponteiro para a instância de `ESP8266WebServer`.
 */
void startServer(ESP8266WebServer* server) {

  if (serverRunning) return;  // evita inicialização duplicada

  serverSetupRoutes(server);
  server->begin();
  serverRunning = true;
}


/**
 * @brief Encerra o servidor HTTP e desativa o Access Point.
 *
 * Esta função interrompe conexões ativas, encerra o serviço HTTP
 * e retorna o módulo Wi-Fi para o modo estação (**WIFI_STA**).
 *
 * @param server Ponteiro para a instância de `ESP8266WebServer`.
 */
void stopServer(ESP8266WebServer* server) {

  if (!serverRunning) return;  // se já estiver parado, não faz nada

  // Encerra o cliente atual, se existir
  if (server->client()) server->client().stop();

  // Para o servidor HTTP
  server->stop();
  serverRunning = false;

  // Desativa o modo Access Point
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

}


/**
 * @brief Mantém o servidor HTTP em execução.
 *
 * Deve ser chamada dentro do `loop()` principal para processar
 * requisições de clientes conectados.
 *
 * @param server Ponteiro para a instância de `ESP8266WebServer`.
 */
void server_handle_loop(ESP8266WebServer* server) {
  server->handleClient();
}
