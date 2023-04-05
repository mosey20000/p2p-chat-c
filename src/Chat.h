#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <ncursesw/curses.h>
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

/// Функция завершает выполнение программы
/// Выведя при этом строку error
void escape(const char* error);

/// Функция нужна для подключения к клиенту
/// Отправляет запрос на подключение, пока не получит ответ
void connectToClient(int sockfd, const struct sockaddr_in* addr, const char* name);

/// Отправляем пакет всем клиентам
void sendPacket(int sockfd, const char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H
