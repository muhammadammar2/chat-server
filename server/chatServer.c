#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // for `sockaddr_in`

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080         
#define MAX_CLIENTS 100

void runServer(int server_socket);
void handleClient(int client_socket);
void cleanupSocket(int socket);

int main() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct sockaddr_in server_addr;
    int server_socket;

    // TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // server address 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind socket to IP and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on Port %d...\n", PORT);
    runServer(server_socket);

    closesocket(server_socket);
    WSACleanup();
    
    return 0;
}

void runServer(int server_socket) {
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int client_socket;

    while (1) {
        // accept new connections using async
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket != INVALID_SOCKET) {
            printf("New client connected.\n");
            handleClient(client_socket);
        } else {
            printf("Accept failed: %d\n", WSAGetLastError());
        }
    }
}

void handleClient(int client_socket) {
    char room_name[100], passcode[100], username[100];

    // ask client for creds
    recv(client_socket, room_name, sizeof(room_name), 0);
    recv(client_socket, passcode, sizeof(passcode), 0);

    int room_id = joinOrCreateRoom(room_name, passcode);  

    recv(client_socket, username, sizeof(username), 0);

    addUserToRoom(room_id, client_socket, username);      

    handleMessaging(client_socket, room_id);              

    cleanupSocket(client_socket);
}

void cleanupSocket(int socket) {
    closesocket(socket);
}
