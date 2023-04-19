#include "Chat.h"

void escape(const char* error) {
    printf("Error!!!\n");
    printf("%s\n", error);
    exit(EXIT_FAILURE);
}

void connect_to_client(int sockfd, const struct sockaddr_in* addr, const char* name) {
    char buf[100];
    int buf_size = 0;

    while (1) {
        buf_size = create_connect_request_packet((char *) &buf, name);
        send_udp(sockfd, addr, buf, buf_size);
        sleep(2);

        struct sockaddr_in buf_address = {0};
        unsigned int address_size = sizeof(struct sockaddr_in); // Оказывается нужна инициализация!!!
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            buf[buf_size] = '\0';
            int packet_id = get_packet_id((char *) &buf);
            if (packet_id == PACKET_CONNECT_ACCEPT && is_equal_address(addr, &buf_address)) {

                char buf_name[MAX_NAME_LENGTH * 2];
                strcpy((char *) &buf_name, buf + 1);
                add_client(&buf_address, (char *) &buf_name);
                update_client_box();

                sprintf((char *) &buf, "Подключились к %s", buf_name);
                add_message((char *) &buf);

                buf_size = create_simple_packet(PACKET_REQUEST_USERS, (char *) &buf);
                send_udp(sockfd, addr, buf, buf_size);
                return;
            }
        }
    }
}

void send_packet(int sockfd, const char* buf, int buf_size) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            send_udp(sockfd, &(clients[i].address), buf, buf_size);
        }
    }
}
