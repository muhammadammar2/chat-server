#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include "../headers/roomManagement.h"

#include "../headers/chatServer.h"


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

    new_room->client_count = 0;
    new_room->clients = NULL;
    new_room->next = rooms;
    rooms = new_room;

    printf("Created new room: %s\n" , room_name);
    return new_room->id;

}

void addUserToRoom(int room_id, int client_socket, const char* username) {
    Room* room = rooms;

    while (room != NULL) {
        if (room->id == room_id) {
            // resize the clients array if necessary
            room->clients = (Client*)realloc(room->clients, (room->client_count + 1) * sizeof(Client));
            if (room->clients == NULL) {
                perror("Failed to reallocate memory for clients");
                return; // exit if realloc fails
            }

            // new client entry
            Client* new_client = &room->clients[room->client_count++];
            new_client->socket = client_socket;
            strncpy(new_client->username, username, sizeof(new_client->username) - 1);
            new_client->username[sizeof(new_client->username) - 1] = '\0'; 

            printf("Added user %s to room %s\n", username, room->name);
            return;
        }
        room = room->next;
    }
    printf("Room ID %d not found\n", room_id);
}



void removeUserFromRoom(int room_id, int client_socket) {
    Room* room = rooms;

    while (room != NULL) {
        if (room->id == room_id) {
            int found_index = -1;

            // find the client to remove
            for (int i = 0; i < room->client_count; ++i) {
                if (room->clients[i].socket == client_socket) {
                    found_index = i;
                    break;
                }
            }

            if (found_index != -1) {
                // shift clients down in the dynamic array
                for (int i = found_index; i < room->client_count - 1; ++i) {
                    room->clients[i] = room->clients[i + 1];
                }
                room->client_count--; 
                room->clients = (Client*)realloc(room->clients, room->client_count * sizeof(Client)); 
                printf("User removed from room %s\n", room->name);
                return;
            } else {
                printf("Client not found in room %s\n", room->name);
                return;
            }
        }
        room = room->next;
    }
    printf("Room ID %d not found\n", room_id);
}

Client* getClientsInRoom(int room_id) {
    Room* room = rooms;

    //find room by ID
    while(room != NULL) {
        if(room->id == room_id) {
            return room->clients;
        }
        room = room->next;
    }
    return NULL;
}

void kickUserFromRoom(int room_id, int admin_socket, int target_socket) {
    Room* room = rooms;

    while (room != NULL) {
        if (room->id == room_id) {
            // check if the admin is a part of the room
            int is_admin = 0;
            for (int i = 0; i < room->client_count; ++i) {
                if (room->clients[i].socket == admin_socket) {
                    is_admin = 1;
                    break;
                }
            }

            if (is_admin) {
                removeUserFromRoom(room_id, target_socket);
                printf("User kicked from room %s\n", room->name);
                return;
            } else {
                printf("User is not admin in room %s\n", room->name);
                return;
            }
        }
        room = room->next;
    }
    printf("Room ID %d not found\n", room_id);
}


void deleteRoom(int room_id) {
    Room** current = &rooms;

    while (*current != NULL) {
        if ((*current)->id == room_id) {
            Room* to_free = *current;
            *current = (*current)->next;

            // free all clients
            free(to_free->clients);
            free(to_free);
            printf("Room ID %d deleted\n", room_id);
            return;
        }
        current = &(*current)->next;
    }
    printf("Room ID %d not found\n", room_id);
}



int findSocketByUsername(int room_id, const char* username) {
    Room* room = getRoomById(room_id);
    if (room == NULL) return -1; //room not founds

    for (int i = 0; i < room->client_count; i++) {
        if (strcmp(room->clients[i].username, username) == 0) {
            return room->clients[i].socket; // return the socket of the target user
        }
    }
    return -1; // user not found
}


Room* getRoomById(int room_id) {
    Room* room = rooms; 
    while (room != NULL) {
        if (room->id == room_id) {
            return room; 
        }
        room = room->next;
    }
    return NULL; 
}


int admin_socket;

bool isAdmin(int client_socket) {
    //if the client is an admin
    return (client_socket == admin_socket); //example
}