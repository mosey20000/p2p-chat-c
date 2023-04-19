#include "Chat.h"
#include "Utils/Utils.h"


int main(int argc, char *argv[]) {
    int connect_port = DEFAULT_PORT;
    int source_port = DEFAULT_PORT;
    char* connect_ip = NULL;
    char name[MAX_NAME_LENGTH] = "";

    parse_connect_adress(argc, argv, &connect_ip, &source_port);
    parse_source_port(argc, argv, &source_port);
    parse_name(argc, argv, (char *) &name);

    if (strcmp((char *) &name, "") == 0) {
        escape("Необходимо ввести имя: -name <имя>");
    }

    fflush(stdin);
    interface_init();

    struct sockaddr_in local_address;
    struct sockaddr_in buf_address;

    char buf_read[BUFLEN] = {0};
    char buf_send[BUFLEN] = {0};
    char buf_name[MAX_NAME_LENGTH] = {0};

    int buf_read_size = 0;
    int buf_send_size = 0;

    int sockfd = create_socket();

    bind_address(sockfd, &local_address, source_port);

    char* source_ip = inet_ntoa(local_address.sin_addr);

    update_info_box((char *) &name, source_ip, source_port);
    add_client(&buf_address, (char *) &name);

    sen_nonblock_flag(sockfd);
    sen_nonblock_flag(0);

    if (connect_ip != NULL) {
        create_adress(connect_ip, connect_port, &buf_address);

        sprintf((char *) &buf_send, "Подключаемся к %s:%d", connect_ip, connect_port);
        add_message((char *) &buf_send);

        connect_to_client(sockfd, &buf_address, (char *) &name);
    } else {
        add_message("Ждем подключения");
    }

    int timeToSendPing = SEND_PING_PAUSE;
    while (1) {
        unsigned int address_size = sizeof(local_address);
        while ((buf_read_size = socket_read(sockfd, (char *) &buf_read, &buf_address, &address_size)) != -1) {
            if (is_equal_address(&local_address, &buf_address)) {
                continue;
            }
            int packet_id = get_packet_id((char *) &buf_read);

            struct Client* client = get_client(&buf_address);

            if (client == NULL && packet_id != PACKET_CONNECT_REQUES && packet_id != PACKET_CONNECT_ACCEPT) {
                continue;
            }

            if (client != NULL) {
                if (packet_id != PACKET_PING) {
                    create_simple_packet(PACKET_PING, (char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, 1);
                }
            }

            buf_read[buf_read_size] = '\0';
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);
            switch (packet_id) {
                case PACKET_CONNECT_REQUES:
                    if (!is_exist(&buf_address)) {
                        strcpy((char *) &buf_name, buf_read + 1);
                        add_client(&buf_address, (char *) &buf_name);
                        if (strcmp(buf_name, name) == 0) {
                            break;
                        }
                        update_client_box();
                        sprintf((char *) &buf_send, "Подключился клиент %s [%s:%d]", buf_name, buf_ip, buf_port);
                        add_message((char *) &buf_send);
                    }
                    buf_send_size = create_connect_accept_packet((char *) &buf_send, (char *) &name);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_CONNECT_ACCEPT:
                    if (!is_exist(&buf_address)) {
                        strcpy((char *) &buf_name, buf_read + 1);
                        add_client(&buf_address, (char *) &buf_name);
                        update_client_box();

                        sprintf((char *) &buf_send, "Подключились к %s", buf_name);
                        add_message((char *) &buf_send);
                    }
                    break;
                case PACKET_PING:
                    client->isActive = PING_SKIP_TO_TIMEOUT;
                    break;
                case PACKET_TIMEOUT:
                    connect_to_client(sockfd, &buf_address, (char *) &name);
                    break;
                case PACKET_SEND_MESSAGE:
                    get_name(client, (char *) &buf_name);
                    if (strcmp(buf_name, name) == 0) {
                        break;
                    }
                    sprintf((char *) &buf_send, "%s: %s", buf_name, buf_read + 1);
                    add_message(buf_send);
                    break;
                case PACKET_REQUEST_USERS:
                    buf_send_size = create_list_users_packet((char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_LIST_USERS:
                    buf_send_size = create_connect_request_packet((char *) &buf_send, (char *) &name);
                    int count =  buf_read[1];
                    for (int i = 0; i < count; i++) {
                        memcpy(&(buf_address), buf_read + 2, sizeof(struct sockaddr_in));
                        send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    }
                    break;
            }
        }
        static int size_input = 0;
        static char buf_input[100] = {0};
        while (read_input_box((char *) buf_input, &size_input) == 1) {
            if (strcmp(buf_input, "/quit") == 0) {
                close_socket(sockfd);
                interface_close();
                return 0;
            }
            sprintf((char *) &buf_send, "Вы: %s", buf_input);
            add_message((char *) &buf_send);
            create_message_packet((char *) &buf_send, (char *) &buf_input, size_input);
            send_packet(sockfd, (char *) &buf_send, size_input + 1);
            memset(buf_input, 0, 100);
            size_input = 0;
        }

        // if (timeToSendPing-- <= 0) {
        //     for (int i = 0; i < 20; i++) {
        //         if (clients[i].isActive == 1) {
        //             create_simple_packet(PACKET_TIMEOUT, (char *) &buf_send);
        //             send_packet(sockfd, (char *) &buf_send, 1);
        //             remove_client(&clients[i]);
        //             sprintf((char *) &buf_send, "Клиент %s отключен. Timeout.", clients[i].name);
        //             add_message((char *) &buf_send);
        //         } else if (clients[i].isActive > 1) {
        //             clients[i].isActive--;
        //             create_simple_packet(PACKET_PING, (char *) &buf_send);
        //             send_packet(sockfd, (char *) &buf_send, 1);
        //         }
        //     }
        //     timeToSendPing = SEND_PING_PAUSE;
        // }
    }
    close_socket(sockfd);
    interface_close();
    return 0;
}