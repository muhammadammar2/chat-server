#ifndef MESSAGING_H
#define MESSAGING_H

void handleMessage(int client_socket, int room_id);
void broadcastMessage(int sender_socket, int room_id, const char *message);

#endif 
