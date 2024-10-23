#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

typedef struct Client {
    int socket;
    char username[100];
    struct Client* next;
} Client;

typedef struct Room {
    int id;
    char name[100] , passcode[100];
    Client* clients;
    struct Room* next;
} Room;

int joinOrCreateRoom(const char* room_name , const char* passcode);

void addUserToRoom(int room_id , int client_socket , const char* username);

void removeUserFroomRoom(int room_id , int client_socket);

Client* getClientsInRoom(int room_id);

void deleteRoom(int room_id);

#endif