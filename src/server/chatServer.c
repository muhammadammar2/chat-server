#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PORT 8080         
#define MAX_CLIENTS 100


void runServer(int server_socket);
void handleClient(int client_socket);

int main () {

    struct sockaddr_in server_addr;
    int server_socket;

    //TCP Socket
    server_socket = socket (AF_INET , SOCK_STREAM , 0);

    if(server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_socket , (struct sockaddr*) & server_addr , sizeof(server_addr)) < 0) {
        perror("Bind failed"); // bind of the socket to IP and PORT
        close (server_socket);
        exit(EXIT_FAILURE);
    }

    if(listen(server_socket , MAX_CLIENTS) < 0) {
        perror ("Listen Failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf ("Server listening on Port %d ... \n" , PORT);
    runServer(server_socket);


    return 0;
}

void runServer (int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket;

    while (1) {
        // accept new connections using async
        client_socket = accept(server_socket , (struct sockaddr*)& client_addr , &client_addr_len);
        if(client_socket >= 0) {
            printf("New client connected. \n");
            handleClient (client_socket);
        } else {
            continue;
        }
    }
}

void handleClient(int client_socket) {
    char room_name[100] , passcode[100] , username[100];

    //ask client for creds
    recv(client_socket , room_name , sizeof(room_name) , 0);
    recv(client_socket , passcode , sizeof(passcode) , 0);

    int room_id = joinOrCreateRoom(room_name , passcode);

    recv(client_socket , username , sizeof(username) , 0);

    addUserToRoom(room_id , client_socket , username);

    handleMessage(client_socket , room_id);
}