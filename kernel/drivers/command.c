#include "command.h"
#include "tty.h"
#include "../libc/string.h"
#include "../libc/utility.h"
#include "../controller/timer.h"
#include <stddef.h>

#define COMMAND_MAXSIZE 128

void command_handle(const uint8_t *command) {
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

    if (strcomp(base_command, "time")) {
        char tick_tmp[32];
        itoa(get_tick() / 60, tick_tmp);

        kprint_color("Uptime: ", BLACK, CYAN);
        kprint_color(tick_tmp, BLACK, CYAN);
        kprint_color(" seconds (ticks: ", BLACK, CYAN);

        itoa(get_tick(), tick_tmp);
        kprint_color(tick_tmp, BLACK, CYAN);
        kprint_color(")", BLACK, CYAN);
    }

    else if (strcomp(base_command, "ping")) {
        kprint_color("Pong!", BLACK, YELLOW);
    }

    else if (strcomp(base_command, "whoami")) {
        kprint_color("default-user", BLACK, WHITE);
    }

    else if (strcomp(base_command, "cls")) {
        clear_screen();
    }

    else if (strcomp(base_command, "lorem")) {
        kprint_color("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", BLACK, LIGHT_CYAN);
    }

    else if (strcomp(base_command, "help")) {
        kprint_color("List of availiable commands:\n", BLACK, LIGHT_GREEN);
        kprint("\ttime   (show uptime in seconds and ticks)\n");
        kprint("\tping   (pings and pongs)\n");
        kprint("\twhoami (show current user)\n");
        kprint("\tcls    (clear screen)\n");
        kprint("\tlorem  (print lorem ipsum)\n");
        kprint("\thelp   (show list of commands)");
    }

    else {
        kprint("Unknown command: ");
        kprint(base_command);
    }
}
