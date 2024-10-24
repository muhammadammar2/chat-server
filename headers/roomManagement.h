#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#include <stdbool.h>

//arrays for clients
typedef struct Client {
    int socket;
    char username[100];
    struct Client* next;
} Client;

// LL for rooms
typedef struct Room {
    int id;
    char name[100] , passcode[100];
    Client* clients;
    int client_count;
    int client_capacity;
    struct Room* next;
} Room;

int findSocketByUsername(int room_id, const char* username);

Room* getRoomById(int room_id);

int JoinOrCreateRoom(const char* room_name , const char* passcode);

void addUserToRoom(int room_id , int client_socket , const char* username);

void removeUserFromRoom(int room_id , int client_socket);

Client* getClientsInRoom(int room_id);

void kickUserFromRoom(int room_id , int admin_socket , int target_socket);

bool isAdmin(int client_socket);

void deleteRoom(int room_id);

#endif