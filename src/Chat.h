#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <curses.h> // Interface in terminal
#include <locale.h> // setLocale()
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strcmp()
#include <unistd.h> // read(), close()
#include <fcntl.h>  // fcntl()
#include <arpa/inet.h> // struct sockaddr_in
#include <sys/ioctl.h> // ioctl()

#include "Config.h"
#include "Clients.h"
#include "Network/Packet.h"
#include "Network/Socket.h"
#include "Utils/Interface.h"

void escape(const char* error);

void connect_to_client(int sockfd, const struct sockaddr_in* addr, const char* name);

void send_packet(int sockfd, const char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H
