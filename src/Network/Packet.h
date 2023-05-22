#ifndef C_P2P_CHAT_PACKET_H
#define C_P2P_CHAT_PACKET_H

#include "../Chat.h"

#define PACKET_CONNECT_REQUES '0'

#define PACKET_CONNECT_ACCEPT '1'

#define PACKET_PING '3'

#define PACKET_TIMEOUT '4'

#define PACKET_REQUEST_USERS '5'
#define PACKET_LIST_USERS '6'

#define PACKET_SEND_MESSAGE '7'

int get_packet_id(const char* data);

int create_simple_packet(char type, char* buf);

int create_connect_request_packet(char* buf, const char* name);
int create_connect_accept_packet(char* buf, const char* name);

int create_message_packet(char* buf_send, char* buf_input, int len_msg);

int create_list_users_packet(char* buf);


#endif //C_P2P_CHAT_PACKET_H
