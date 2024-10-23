#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

typedef struct Client {
    int socket;
    char username[100];
    struct Client* next;
} Client;