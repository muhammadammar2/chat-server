#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <winsock2.h>


#include "../headers/messaging.h"
#include "../headers/roomManagement.h"

void handleClient(int client_socket);

void cleanupClient(int client_socket);

void handleInput (int socket);

// int JoinOrCreateRoom(const char* room_name , const char* passcode);

// void addUserToRoom(int room_id , int client_socket , const char* username);

// void broadcastMessage(int sender_socket, int room_id, const char *message);

#endif 
