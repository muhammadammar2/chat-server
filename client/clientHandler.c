#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "../headers/chatServer.h"

#include "../headers/clientHandler.h"

#include "../headers/socketUtils.h"

#include "../headers/roomManagement.h" 

#include "../headers/messaging.h"

#define SERVER_IP "127.0.0.1" 

#define PORT 8080

// void handleInput (int socket) {
//     char command [256];

//     while (1) {
//         printf("Start typing: ");   
//         fgets(command , sizeof(command) , stdin);
//         command[strcspn(command, "\n")] = 0;

//         send(socket , command , strlen(command) , 0);
//     }
// }

void handleInput(int socket) {
    char command[256];

    printf("Hey there, welcome!\n");
    printf("Do you want to create a room or join an existing one?\n");
    printf("Type 'CREATE <room_name> <passcode>' to create a room\n");
    printf("Type 'JOIN <room_name> <passcode>' to join a room\n");
    printf("Type 'EXIT' to disconnect from the server.\n");

    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == 0) {
            continue; 
        }

        int bytes_sent = send(socket, command, strlen(command), 0);
        if (bytes_sent == SOCKET_ERROR) {
            perror("Failed to send command");
            break; 
        }

    }
}



void handleClient(int client_socket) {
    char buffer[1024] , username[100] ,room_name[100] , passcode[100];

    recv(client_socket, username, sizeof(username), 0);

    recv(client_socket, room_name, sizeof(room_name), 0);
    recv(client_socket, passcode, sizeof(passcode), 0);

    int room_id = JoinOrCreateRoom(room_name, passcode);
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

        broadcastMessage(client_socket , room_id , buffer);
    }

    cleanupClient(client_socket);

}

void cleanupClient(int client_socket){
    closesocket(client_socket);
    printf("Client socket closed.\n");
}