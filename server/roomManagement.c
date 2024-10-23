#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roomManagement.h"


static Room* rooms = NULL;
static int next_room_id = 1;

int JoinOrCreateRoom(const char* room_name , const char* passcode) {
    Room* room = rooms;

    while(room != NULL) {
        if(strcmp(room->name , room_name) == 0) {
            if(strcmp(room->passcode , passcode) == 0) {
                return room->id;
            } else {
                printf("Wrong passcode for the room %s\n" , room_name);
                return -1;
            }
        }
        room = room->next;
    }

    //doesn exist , create a new one
    Room* new_room = (Room*) malloc(sizeof(Room));
    new_room->id = next_room_id++;
    strcpy(new_room->name , room_name);
    strcpy(new_room->passcode , passcode);

    new_room->clients = NULL;
    new_room->next = rooms;
    rooms = new_room;

    printf("Created new room: %s\n" , room_name);
    return new_room->id;

}

void addUserToRoom(int room_id , int client_socket , const char* username) {
    Room* room = rooms;

    //find it by roomID
    while(room != NULL) {
        if(room->id == room_id) {
            // create a new client
            Client* new_client = (Client*) malloc(sizeof(Client));
            new_client->socket = client_socket;
            strcpy(new_client->username , username);
            new_client->next = room->clients;
            room->clients = new_client;

            printf("Added user %s to room %s\n" , username , room->name);
            return;
        }
        room = room->next;
    }
    printf("Room Id %d not found\n" , room_id);
}