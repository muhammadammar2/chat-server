#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


void runServer(int server_socket);
void handleClient(int client_socket);


void handleClientMessage(int client_socket , int admin_socket , const char* message);

// boolean isAdmin(int client_socket);


#endif