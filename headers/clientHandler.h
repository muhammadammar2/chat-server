#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>

void handleClient(int client_socket);

void cleanupClient(int client_socket);

void handleInput (int socket);

#endif 
