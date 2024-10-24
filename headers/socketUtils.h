#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <winsock2.h>

int createSocket();

void bindSocket(int server_socket , int port);

void listenForConnections(int server_socket);


int acceptConnections(int serever_socket , struct sockaddr_in * client_addr);

void cleanupSocket(int socket);

#endif