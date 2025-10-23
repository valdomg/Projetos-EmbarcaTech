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
