#ifndef C_P2P_CHAT_CLIENTS_H
#define C_P2P_CHAT_CLIENTS_H

#include "Chat.h"

struct Client {
    int isActive;
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in address;
};
extern struct Client clients[MAX_CLIENTS];


void add_client(const struct sockaddr_in* addr, const char* name);

struct Client* get_client(const struct sockaddr_in* addr);

int is_exist(const struct sockaddr_in* addr);

void remove_client(struct Client* client);

void get_name(const struct Client* client, char* name);

#endif //C_P2P_CHAT_CLIENTS_H
