#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h> // For `sockaddr_in`

#include "socketUtils.h"

#pragma comment(lib, "Ws2_32.lib")


int createSocket() {
    int server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if(server_socket < 0) {
        perror("Socket connection failed");
        exit(EXIT_FAILURE);
    } 
    return server_socket;
}

void bindSocket(int server_socket , int port) {
    
}