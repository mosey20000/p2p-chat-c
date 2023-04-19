#ifndef C_P2P_CHAT_UTILS_H
#define C_P2P_CHAT_UTILS_H

#include "../Chat.h"

void parse_connect_adress(int argc, char *argv[], char** ip, int* port);
void parse_source_port(int argc, char *argv[], int* port);
void parse_name(int argc, char *argv[], char* name);

#endif //C_P2P_CHAT_UTILS_H
