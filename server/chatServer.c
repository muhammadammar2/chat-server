#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // for `sockaddr_in`

// link with Ws2_32.lib
// #pragma comment(lib, "Ws2_32.lib")

#define PORT 8080         
#define MAX_CLIENTS 100

void runServer(int server_socket);
void handleClient(int client_socket);
void cleanupClients(int* clients , int count);

int main() {
    struct sockaddr_in server_addr;   
    int server_socket;

    //TCP socket
    server_socket = createSocket();
    bindSocket(server_socket , PORT);
    listenForConnections(server_socket);

    printf("Server listening on Port %d ... \n" , PORT);
    runServer(server_socket);

    cleanupSocket(server_socket);
    return 0;
}


void runServer(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket;

    int* clients = malloc(sizeof(int) * MAX_CLIENTS);
    int client_count = 0;

    while(1) {
        client_socket = acceptConnection(server_socket , &client_addr);
        if(client_socket >= 0) {
            if(client_socket >= MAX_CLIENTS) {
                printf("Max clients reached. Connection refused. \n");
                closesocket(client_socket);
                continue;
            }

            printf("New client connected. \n");
            clients[client_count++] = client_socket;

            //resizing array to double if needed obv
            if(client_count % MAX_CLIENTS == 0) {
                clients = realloc(clients , sizeof(int) * (client_count + MAX_CLIENTS));
            }

            handleClient(client_socket);
        }
    }

    cleanupSocket(clients , client_count);
    free(clients);
}

void handleClient(int client_socket) {
    char room_name[100] , passcode[100] , username[100];

    //ask clients for creds
    recv(client_socket , room_name , sizeof(room_name) , 0);
    recv(client_socket , passcode , sizeof(passcode) , 0);

    int room_id = joinOrCreateRoom(room_name , passcode);
    if(room_id == -1) {
        closesocket(client_socket);
        return;
    }

    recv(client_socket , username , sizeof(username) , 0);
    addUserToRoom(room_id , client_socket , username);

    handleMessage(client_socket , room_id);
}

void cleanupClients(int* clients , int count) {
    for(int i = 0; i < count; ++i) {
        closesocket(clients[i]);
    }
}