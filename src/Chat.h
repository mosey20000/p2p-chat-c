#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <curses.h>
#include <locale.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>  
#include <arpa/inet.h> 
#include <sys/ioctl.h> 

#include "Config.h"
#include "Clients.h"
#include "Network/Packet.h"
#include "Network/Socket.h"
#include "Utils/Interface.h"

void escape(const char* error);

void connect_to_client(int sockfd, const struct sockaddr_in* addr, const char* name);

void send_packet(int sockfd, const char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H
