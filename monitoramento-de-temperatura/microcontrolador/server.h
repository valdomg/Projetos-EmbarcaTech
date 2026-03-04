/**
 * @file server.h
 * @brief Interface do módulo responsável pelo servidor HTTP embarcado no ESP8266.
 *
 * Este módulo fornece funções para criação, configuração, execução e encerramento
 * de um servidor web HTTP no microcontrolador ESP8266.
 *
 * O servidor é utilizado principalmente para:
 *
 * - Disponibilizar uma interface web de configuração do dispositivo;
 * - Receber parâmetros enviados pelo usuário através de formulários HTML;
 * - Permitir configuração de rede Wi-Fi, parâmetros MQTT e limites ambientais;
 * - Controlar o ciclo de vida do servidor durante o modo de configuração.
 *
 * Funcionalidades principais:
 *
 * - Inicialização do servidor HTTP;
 * - Registro de rotas e endpoints;
 * - Processamento de requisições de clientes;
 * - Encerramento seguro do servidor;
 * - Suporte ao modo Access Point (AP) para configuração local.
 *
 * Fluxo típico de utilização:
 *
 * @code
 * ESP8266WebServer server(80);
 *
 * startServer(&server);
 *
 * void loop() {
 *     server_handle_loop(&server);
 * }
 * @endcode
 *
 * Dependências:
 *
 * - Biblioteca ESP8266WebServer
 * - Módulo de configuração do sistema
 *
 * @note Este módulo é utilizado principalmente durante o modo de configuração do dispositivo.
 *
 * @warning O servidor deve ser processado continuamente no loop principal para garantir
 * o atendimento das requisições HTTP.
 *
 * @see server.cpp
 */

#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WebServer.h>  // Biblioteca para criar e gerenciar um servidor HTTP no ESP8266

/**
 * @brief Inicia o servidor HTTP e configura as rotas de acesso.
 * 
 * Essa função chama as rotas definidas em server_setup_routes()
 * e inicia o serviço do servidor. Deve ser chamada após configurar o modo Access Point (AP)
 * ou conectar o dispositivo a uma rede Wi-Fi.
 * 
 * @param server Ponteiro para a instância de ESP8266WebServer a ser iniciada.
 */
void startServer(ESP8266WebServer *server);

/**
 * @brief Encerra o servidor HTTP e desconecta o Access Point.
 * 
 * Essa função interrompe o atendimento de requisições HTTP,
 * encerra a conexão com os clientes e desativa o modo AP do Wi-Fi.
 * 
 * @param server Ponteiro para a instância de ESP8266WebServer a ser parada.
 */
void stopServer(ESP8266WebServer *server);

/**
 * @brief Configura as rotas HTTP do servidor.
 * 
 * Define as páginas e endpoints que o servidor responderá.
 * Normalmente, inclui a rota raiz ("/") e a rota de envio de formulário ("/submit").
 * 
 * @param server Ponteiro para a instância de ESP8266WebServer onde as rotas serão registradas.
 */
void server_setup_routes(ESP8266WebServer* server);

/**
 * @brief Mantém o servidor ativo e processa requisições de clientes.
 * 
 * Essa função deve ser chamada no loop principal do programa (`loop()`),
 * garantindo que o servidor trate novas conexões e requisições HTTP.
 * 
 * @param server Ponteiro para a instância de ESP8266WebServer em execução.
 */
void server_handle_loop(ESP8266WebServer* server);

#endif
