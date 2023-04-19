#include "Interface.h"

static WINDOW* box_info = NULL;
static WINDOW* box_client = NULL;
static WINDOW* box_messages = NULL;
static WINDOW* box_input = NULL;

static char messages[16][126] = {{0}};

static void init_info_box() {
    box_info = newwin(5, 65, 0, 0);
    box(box_info, 0, 0);
    wrefresh(box_info);
}

static void init_client_box() {
    box_client = newwin(25, 15, 0, 65);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    wrefresh(box_client);
}

static void init_message_box() {
    box_messages = newwin(17, 65, 5, 0);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 16, 0, "│                                                               │");

    wrefresh(box_messages);
}

static void init_inpit_box() {
    box_input = newwin(3, 65, 22, 0);
    box(box_input, 0, 0);
    mvwprintw(box_input, 0, 0, "├───────────────────────────────────────────────────────────────┤");;

    wrefresh(box_input);
}

void update_client_box() {
    wclear(box_client);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    int position = 3;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            mvwprintw(box_client, position, 1, clients[i].name);
            position++;
        }
    }

    wrefresh(box_client);
}

static void update_message_box() {
    wclear(box_messages);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 16, 0, "│                                                               │");
    for (int i = 0; i < 16; i++) {
        mvwprintw(box_messages, i + 1, 1, messages[i]);
    }

    wrefresh(box_messages);
}

void add_message(const char* msg) {
    for (int i = 1; i < 16; i++) {
        memset((char *) &messages[i - 1], ' ', sizeof(char) * 18);
        strcpy((char *) &messages[i - 1], (char *) &messages[i]);
    }
    strcpy((char *) &messages[15], msg);
    update_message_box();
}


void update_info_box(const char* name, const char* ip, int port) {
    wclear(box_info);
    box(box_info, 0, 0);
    mvwprintw(box_info, 2, 1, "  Ваш ник: ");
    mvwprintw(box_info, 2, 13, name);

    wrefresh(box_info);
}

void interface_init() {
    setlocale(LC_ALL,"");
    // Изменяем размер экрана
    printf("\e[8;25;80;t");

    initscr();

    init_info_box();
    init_message_box();
    init_client_box();
    init_inpit_box();

    keypad(box_input, TRUE);
    echo();
    cbreak();      // disable line-buffering
    wtimeout(box_input, 1000 / TICK_PER_SECOND);  // wait 100 milliseconds for input
}

int read_input_box(char* buf, int* size) {
    int symbol = 0;
    while ((symbol = wgetch(box_input)) != ERR) {
        if (symbol == '\n') {
            for (int i = 0; i < *size; i++) {
                mvwprintw(box_input, 1, i + 1, " ");
            }
            return 1;
        }else if (symbol == KEY_BACKSPACE) {
            if (*size > 0) {
                mvwprintw(box_input, 1, (*size), " ");
                buf[--(*size)] = 0;
            }
        }else if (*size < 99) {
            buf[(*size)++] = (char) symbol;
        }
    }
    mvwprintw(box_input, 1, 1, (char *)buf);
    return 0;
}

void interface_close() {
    delwin(box_info);
    delwin(box_client);
    delwin(box_client);
    delwin(box_messages);

    endwin();
}