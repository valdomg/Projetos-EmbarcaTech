#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WebServer.h>


void startServer(ESP8266WebServer *server);

void stopServer(ESP8266WebServer *server);

// Configura as rotas do servidor
void server_setup_routes(ESP8266WebServer* server);

// Loop do servidor
void server_handle_loop(ESP8266WebServer* server);


#endif