#ifndef C_P2P_CHAT_INTERFACE_H
#define C_P2P_CHAT_INTERFACE_H

#include "../Chat.h"

void interface_init();
void interface_close();

void update_client_box();
void update_info_box(const char* name, const char* ip, int port);

void add_message(const char* msg);

int read_input(char* buf, int* size);

#endif //C_P2P_CHAT_INTERFACE_H
