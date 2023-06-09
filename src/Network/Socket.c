#include "Socket.h"

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) escape("Can't create socket");
    return sockfd;
}

void close_socket(int sockfd) {
    close(sockfd);
}

void bind_address(int sockfd, struct sockaddr_in *addr, int port) {
    memset((char *) addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons((unsigned short) port); // To network byte order
    addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int result = bind(sockfd, (struct sockaddr *) addr, sizeof(*addr));
    if (result == -1) escape("Can't bind address");
}

void send_udp(int sockfd, const struct sockaddr_in *addr, const char *buf, int buf_size) {
    sendto(sockfd, buf, (size_t) buf_size, 0, (struct sockaddr*) addr, sizeof(*addr));
}

int socket_read(int sockfd, char *buf, struct sockaddr_in* addr, unsigned int *addr_len) {
    int recv_len = (int) recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*) addr, addr_len);
    return recv_len;
}

void set_nonblock_flag(int descriptor) {
    int flags = fcntl(descriptor, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(descriptor, F_SETFL, flags);
}

int is_equival_address(const struct sockaddr_in* first, const struct sockaddr_in* second) {
    return  (first->sin_addr.s_addr == second->sin_addr.s_addr) &&
            (first->sin_port == second->sin_port);
}

void create_address(const char* ip, int port, struct sockaddr_in* addr) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
}