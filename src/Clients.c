#include "Clients.h"


// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct Client clients[MAX_CLIENTS] = {0};


void add_client(const struct sockaddr_in* addr, const char* name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive <= 0) {
            // Копируем адрес
            memcpy(&(clients[i].address), addr, sizeof(struct sockaddr_in));
            strcpy((char *) &(clients[i].name), name);
            clients[i].isActive = PING_SKIP_TO_TIMEOUT;
            update_client_box();
            return;
        }
    }
}

struct Client* get_client(const struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            // Сравниваем ip
            if (is_equal_address(addr, &(clients[i].address))) {
                return &(clients[i]);
            }
        }
    }
    return NULL;
}

int is_exist(const struct sockaddr_in* addr) {
    return get_client(addr) != NULL;
}

void remove_client(struct Client* client) {
    client->isActive = 0;
    update_client_box();
}

void get_name(const struct Client* client, char* name) {
    strcpy(name, (char *) &(client->name));
}