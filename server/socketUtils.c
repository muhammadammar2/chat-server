#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h> // For `sockaddr_in`

#include "../headers/socketUtils.h"

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


int createSocket() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2 , 2) , &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed: %d\n" , iResult);
        exit (EXIT_FAILURE);
    }
   // create tcp socket
    int server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if(server_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n" , WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return server_socket;
}


void bindSocket(int server_socket , int port) {
    struct sockaddr_in server_addr;
    //for configuration of server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(server_socket , (struct sockaddr*)&server_addr , sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void listenForConnections(int server_socket) {
    if(listen(server_socket , 10) == SOCKET_ERROR) {
        printf("Listen failed: %d\n" , WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

int acceptConnections (int server_socket , struct sockaddr_in *client_addr) {
    int client_addr_ln = sizeof(*client_addr);
    int client_socket = accept(server_socket , (struct sockaddr*)client_addr , &client_addr_ln);
    if(client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n" , WSAGetLastError());
    }
    return client_socket;
}

void cleanupSocket(int socket) {
    closesocket(socket);
    WSACleanup();
}