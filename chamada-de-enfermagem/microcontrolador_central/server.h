/**
 * @file server.h
 * @brief Interface do módulo de servidor HTTP para configuração do dispositivo.
 *
 * @details
 * Este módulo define a interface do servidor HTTP embarcado utilizado
 * para configurar o dispositivo através de uma interface web.
 *
 * O servidor permite que o usuário configure parâmetros essenciais
 * do sistema, como credenciais de rede Wi-Fi e informações de conexão
 * com o broker MQTT, diretamente por um navegador.
 *
 * ## Funcionalidades principais
 *
 * - Inicialização do servidor HTTP
 * - Configuração das rotas de acesso (endpoints)
 * - Processamento de requisições HTTP
 * - Encerramento do servidor e limpeza de recursos
 *
 * ## Fluxo de uso
 *
 * Normalmente o servidor é utilizado da seguinte forma:
 *
 * 1. Criar uma instância de `ESP8266WebServer`.
 * 2. Iniciar o servidor utilizando `startServer()`.
 * 3. No `loop()` principal, chamar `server_handle_loop()` para
 *    processar requisições HTTP.
 * 4. Quando necessário, parar o servidor com `stopServer()`.
 *
 * ## Dependências
 *
 * Este módulo utiliza a biblioteca:
 *
 * - ESP8266WebServer
 *
 * responsável por implementar o servidor HTTP no ESP8266.
 *
 * @note
 * O servidor é geralmente utilizado no modo de configuração
 * do dispositivo, permitindo que o usuário forneça parâmetros
 * necessários para o funcionamento do sistema.
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
