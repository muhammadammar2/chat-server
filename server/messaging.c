#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "../headers/roomManagement.h"

#include "../headers/messaging.h"

#define BUFFER_SIZE 1024

void broadcastMessage(int sender_socket, int room_id, const char* message) {
    Room* room = getRoomById(room_id);
    if (room == NULL) {
        printf("Room not found.\n");
        return;
    }

    for (int i = 0; i < room->client_count; ++i) {
        int client_socket = room->clients[i].socket; // access the socket through the client struct

        if (client_socket != sender_socket) {
            send(client_socket, message, strnlen(message, BUFFER_SIZE), 0);
        }
    }
}

void handleMessage(int client_socket, int room_id) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // clear

        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            removeUserFromRoom(room_id, client_socket); 
            closesocket(client_socket);
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received message from client in room %d: %s\n", room_id, buffer);

        broadcastMessage(client_socket, room_id, buffer);
    }
}
