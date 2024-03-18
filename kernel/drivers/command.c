#include <stddef.h>
#include "command.h"
#include "tty.h"
#include "ports.h"
#include "../controller/timer.h"
#include "../controller/speaker.h"
#include "../libc/string.h"
#include "../libc/utility.h"

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
        itoa(calc_second(get_tick()), tick_tmp);
        kprint_color("Uptime: ", BLACK, CYAN);
        kprint_color(tick_tmp, BLACK, CYAN);
        kprint_color(" seconds ", BLACK, CYAN);

        kprint_color("(IRQ ticks: ", BLACK, BROWN);
        itoa(get_tick(), tick_tmp);
        kprint_color(tick_tmp, BLACK, BROWN);
        kprint_color(")", BLACK, BROWN);
    }

    else if (strcmp(base_command, "ping") == 0) {
        kprint_color("Pong!", BLACK, YELLOW);
    }

    else if (strcmp(base_command, "whoami") == 0) {
        kprint_color("kernel", BLACK, WHITE);
    }

    else if (strcmp(base_command, "cls") == 0) {
        clear_screen();
    }

    else if (strcmp(base_command, "exit") == 0) {
        outw(0x604, 0x2000);  // Temporary solution for QEMU
    }

    else if (strcmp(base_command, "help") == 0) {
        kprint_color("List of available commands:\n", BLACK, LIGHT_GREEN);
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
