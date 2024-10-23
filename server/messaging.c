#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "roomManagement.h"
#include "messaging.h"
#define BUFFER_SIZE 1024


void broadcastMessage(int sender_socket , int room_id , const char* message) {
    Room* room = getRoomById(room_id);
    if(room == NULL) {
        printf("Room not found/ \n");
        return;
    }

    for(int i = 0; i < room->client_count; ++i) {
        int client_socket = room->clients[i];

        if(client_socket != sender_socket) {
            send(client_socket , message , strnlen(message) , 0);
        }
    }
}


void handleMessage(int client_socket , int room_id) {
    char buffer[BUFFER_SIZE];
    int bytes_recieved;

    while(1) {
        memset(buffer , 0 , BUFFER_SIZE); // clear the buffer

        bytes_recieved = recv(client_socket , buffer , BUFFER_SIZE , 0);

        if(bytes_recieved <= 0) {
            printf("Client disconnected or error occured. \n");
            removeClientFromRoom(room_id , client_socket);
            closesocket(client_socket);
            break;
        }
        buffer[bytes_recieved] = '\0';
        printf("Recieved message from client in room %d: %s\n" , room_id , buffer);

        broadcastMessage(client_socket , room_id , buffer);
    }
}






