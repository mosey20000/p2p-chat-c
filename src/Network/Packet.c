#include "Packet.h"

int get_packet_id(const char* buf) {
    return buf[0];
}

int create_simple_packet(char type, char* buf) {
    buf[0] = type;
    return 1;
}

int create_connect_request_packet(char* buf, const char* name) {
    buf[0] = PACKET_CONNECT_REQUES;
    strcpy(buf + 1, name);
    return 1 + (int) strlen(name);
}

int create_connect_accept_packet(char* buf, const char* name) {
    buf[0] = PACKET_CONNECT_ACCEPT;
    strcpy(buf + 1, name);
    return 1 + (int) strlen(name);
}

int create_message_packet(char* buf_send, char* buf_input, int len_msg) {
    buf_send[0] = PACKET_SEND_MESSAGE;
    strcpy(buf_send + 1, buf_input);
    return 1 + len_msg;
}

int create_list_users_packet(char* buf) {
    buf[0] = PACKET_LIST_USERS;
    buf[1] = 0; // Кол-во клиентов

    int pos = 2;
    // Для всех клиентов
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            buf[1]++;
            memcpy(buf + pos, &(clients->address), sizeof(struct sockaddr_in));
            pos += sizeof(struct sockaddr_in);
        }
    }
    return pos;
}
