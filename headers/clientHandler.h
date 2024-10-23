#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>

void handleClient(int client_socket);

// void broadcastMessage(int room_id , const char* message);

void kickUser(int room_id , int client_socket , const char* username);

void cleanupClient(int client_socket);

#endif 
