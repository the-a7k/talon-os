#include <stddef.h>
#include "../include/command.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/timer.h"
#include "../include/speaker.h"
#include "../include/string.h"
#include "../include/utility.h"

#define COMMAND_MAXSIZE 128

void command_handle(const char *command) {
    char base_command[COMMAND_MAXSIZE];
    strwipe(base_command);

    size_t i = 0;
    while (command[i] != '\0') {
        if (command[i] == ' ')
            break;
        charcat(base_command, command[i]);
        i++;
    }

    strtolower(base_command);

    if (strcmp(base_command, "time") == 0) {
        char tick_tmp[32];
        itoa(tick_calc_sec(tick_get()), tick_tmp);
        kprint_color("Uptime: ", TTY_BLACK, TTY_CYAN);
        kprint_color(tick_tmp, TTY_BLACK, TTY_CYAN);
        kprint_color(" seconds ", TTY_BLACK, TTY_CYAN);

        kprint_color("(IRQ ticks: ", TTY_BLACK, TTY_BROWN);
        itoa(tick_get(), tick_tmp);
        kprint_color(tick_tmp, TTY_BLACK, TTY_BROWN);
        kprint_color(")", TTY_BLACK, TTY_BROWN);
    }

    else if (strcmp(base_command, "ping") == 0) {
        kprint_color("Pong!", TTY_BLACK, TTY_YELLOW);
    }

    else if (strcmp(base_command, "whoami") == 0) {
        kprint_color("kernel", TTY_BLACK, TTY_WHITE);
    }

    else if (strcmp(base_command, "cls") == 0) {
        clear_screen();
    }

    else if (strcmp(base_command, "exit") == 0) {
        outw(0x604, 0x2000);  // Temporary solution for QEMU
    }

    else if (strcmp(base_command, "help") == 0) {
        kprint_color("List of available commands:\n", TTY_BLACK, TTY_LIGHT_GREEN);
        kprint("\ttime   (show uptime in seconds and ticks)\n");
        kprint("\tping   (pings and pongs)\n");
        kprint("\twhoami (show current user)\n");
        kprint("\tcls    (clear screen)\n");
        kprint("\texit   (shut down VM machine)\n");
        kprint("\thelp   (show list of commands)");
    }

    else {
        kprint("Unknown command: ");
        kprint(base_command);
    }
}
