#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "clientHandler.h"
#include "socketUtils.h"

void handleClient(int client_socket) {
    char buffer[1024] , username[100] ,room_name[100] , passcode[100];

    recv(client_socket, username, sizeof(username), 0);

    recv(client_socket, room_name, sizeof(room_name), 0);
    recv(client_socket, passcode, sizeof(passcode), 0);

    int room_id = joinOrCreateRoom(room_name, passcode);
    if (room_id == -1) {
        closesocket(client_socket);
        return; 
    }

    addUserToRoom(room_id, client_socket, username);

    while(1) {
        int bytes_recieved = recv(client_socket , buffer , sizeof(buffer) , 0);
        if(bytes_recieved <= 0) {
            printf("%s has disconnected. \n" , username);
            break;
        }

        //actually we use it to store the stuff before passing to clients
        buffer[bytes_recieved] = '\0';
        printf("%s: %s\n" , username , buffer);

        broadcastMessage(room_id , buffer);
    }

    cleanupClient(client_socket);

}

void kickUser(int room_id , int client_socket , const char* username) {

}

void cleanupClient(int client_socket){
    closesocket(client_socket);
    printf("Client socket closed.\n");
}