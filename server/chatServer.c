#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h> // for `sockaddr_in`
#include "roomManagement.h"

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080         
#define MAX_CLIENTS 100

void runServer(int server_socket);
void handleClient(int client_socket);
// void cleanupClients(int* clients , int count);

int main() {
    struct sockaddr_in server_addr;
    int server_socket, client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // actually creating socket using utility functions
    server_socket = createSocket();
    bindSocket(server_socket, PORT);
    listenForConnections(server_socket);

    printf("Server listening on Port %d ... \n", PORT);
    runServer(server_socket);
    while (1) {
        // accept a new client
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            printf("Failed to accept client connection.\n");
            continue;
        }
        
        printf("Client connected.\n");

        //recieve the messages now
        char buffer[256];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // null-terminate the received message
            handleClientMessage(client_socket,client_socket, buffer);  // handle the client's message
        }
    }

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


void handleClientMessage(int client_socket, int admin_socket , const char* message) {
    char command[256];
    scanf(message, "%s", command); 

    if (strcmp(command, "JOIN") == 0) {
        // handle join room
        char room_name[100], passcode[100];
        scanf(message + strlen(command) + 1, "%s %s", room_name, passcode);
        int room_id = JoinOrCreateRoom(room_name, passcode);
        send(client_socket, (room_id >= 0) ? "Joined room successfully.\n" : "Failed to join room.\n", strlen("Joined room successfully.\n"), 0);
    } else if (strcmp(command, "SEND") == 0) {
        // handle send message
        int room_id;
        char msg[200];
        scanf(message + strlen(command) + 1, "%d %[^\n]", &room_id, msg);
        broadcastMessage(client_socket, room_id, msg);
    } else if (strcmp(command, "KICK") == 0) {
        // handle kick user (admin only)
        if (!isAdmin(client_socket)) {
            send(client_socket, "You do not have permission to kick users.\n", strlen("You do not have permission to kick users.\n"), 0);
            return;
        }

        int room_id;
        char username[100];
        scanf(message + strlen(command) + 1, "%d %s", &room_id, username);
        kickUserFromRoom(room_id ,admin_socket, username);
    } else if (strcmp(command, "LEAVE") == 0) {
        // handle leave room
        int room_id;    
        scanf(message + strlen(command) + 1, "%d", &room_id);
        removeUserFromRoom(room_id, client_socket);
    } else if (strcmp(command, "LIST") == 0) {
        // handle list users
        int room_id;
        scanf(message + strlen(command) + 1, "%d", &room_id);
        Room* room = getRoomById(room_id);
        if (room == NULL) {
            send(client_socket, "Room not found.\n", strlen("Room not found.\n"), 0);
            return;
        }

        // user list response
        char response[1024];
        strcpy(response, "Users in room:\n");
        for (int i = 0; i < room->client_count; ++i) {
            strcat(response, room->clients[i].username); 
            strcat(response, "\n");
        }
        send(client_socket, response, strlen(response), 0);
    } else {
        send(client_socket, "Unknown command.\n", strlen("Unknown command.\n"), 0);
    }
}
