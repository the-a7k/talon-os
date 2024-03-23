#include <stddef.h>
#include "../include/command.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/timer.h"
#include "../include/speaker.h"
#include "../include/string.h"
#include "../include/utility.h"
#include "../include/queue.h"

#define COMMAND_MAXSIZE 128
#define COMMAND_HISTORY_MAXSIZE 16

static char command_buffer[COMMAND_MAXSIZE];
static char *command_history[COMMAND_HISTORY_MAXSIZE];
static bool uppercase_flag = false;


static void cmd_help() {
    kprint_color("List of available commands:\n", TTY_BLACK, TTY_LIGHT_GREEN);
    kprint("\ttime      (show uptime in seconds and ticks)\n");
    kprint("\tsleep     (beep and pause the CPU for 3 seconds)\n");
    kprint("\twhoami    (show current user)\n");
    kprint("\tcls       (clear screen)\n");
    kprint("\tcrash     (crash the system with ISR handler)\n");
    kprint("\tshutdown  (shut down VM machine)\n");
    kprint("\thelp      (show list of commands)");
}


static void cmd_time() {
    char tick_tmp[32];
    itoa(timer_calc_sec(timer_get_tick()), tick_tmp);
    kprint_color("Uptime: ", TTY_BLACK, TTY_CYAN);
    kprint_color(tick_tmp, TTY_BLACK, TTY_CYAN);
    kprint_color(" seconds ", TTY_BLACK, TTY_CYAN);

    kprint_color("(IRQ ticks: ", TTY_BLACK, TTY_BROWN);
    itoa(timer_get_tick(), tick_tmp);
    kprint_color(tick_tmp, TTY_BLACK, TTY_BROWN);
    kprint_color(")", TTY_BLACK, TTY_BROWN);
}


static void cmd_sleep() {
    kprint_color("Sleeping for 3 seconds...\n", TTY_BLACK, TTY_MAGENTA);
    speaker_play(500,100);
    speaker_play(800,100);
    cpu_sleep(3000-200);
    kprint_color("Resumed!", TTY_BLACK, TTY_LIGHT_MAGENTA);
    speaker_play(800,100);
    speaker_play(500,100);
}


static void cmd_whoami() {
    kprint_color("kernel", TTY_BLACK, TTY_WHITE);
}


static void command_backspace() {
    if (strlen(command_buffer)) {
        cursor_retreat();
        clear_cell_cursor();
        strpop(command_buffer);
    }
}


static void command_overflow_handler(keyboard_t *ctx) {
    if (!ctx)
        return;

    queue_element_t process_command = 0;
    if (queue_get_front(&ctx->key_buffer, &process_command)) {
        if (process_command == KEY_ENTER)
            command_execute(command_buffer);
        else if (process_command == KEY_BACKSPACE) {
            command_backspace();
        }
    }
    
    // Keyboard buffer reset
    queue_init(&ctx->key_buffer);
}


void command_key_handler(keyboard_t *ctx) {
    if (!ctx || queue_is_empty(&ctx->key_buffer))
        return;
    
    queue_element_t current_key = 0;
    char converted = 0;

    while (queue_get_front(&ctx->key_buffer, &current_key)) {
        if (strlen(command_buffer) == COMMAND_MAXSIZE - 1) {
            command_overflow_handler(ctx);
            return;
        }

        else if (keyboard_to_char(current_key, &converted)) {
            if (uppercase_flag)
                chartoupper(&converted);
            kputchar(converted);
            charcat(command_buffer, converted);
        }

        else if (keyboard_is_special(current_key)) {
            switch (current_key) {
                case(KEY_CAPSLOCK):
                case(KEY_LSHIFT):
                case(KEY_LSHIFT_UP):
                case(KEY_RSHIFT):
                case(KEY_RSHIFT_UP):
                    uppercase_flag = !uppercase_flag;
                    break;
                case(KEY_BACKSPACE):
                    command_backspace();
                    break;
                case(KEY_ENTER):
                    if (strlen(command_buffer))
                        command_execute(command_buffer);
                    else 
                        newline();
                    break;
                case(KEY_TAB):
                    tab();
                    break;
            }
        }
        queue_pop(&ctx->key_buffer);
    }
}


void command_execute(const char *command) {
    char base_command[COMMAND_MAXSIZE];
    strwipe(base_command);
    newline();

    size_t i = 0;
    while (command[i] != '\0') {
        if (command[i] == ' ')
            break;
        charcat(base_command, command[i]);
        i++;
    }
    strtolower(base_command);

    if (strcmp(base_command, "time") == 0)
        cmd_time();

    else if (strcmp(base_command, "whoami") == 0)
        cmd_whoami();

    else if (strcmp(base_command, "cls") == 0)
        clear_screen();

    else if (strcmp(base_command, "sleep") == 0)
        cmd_sleep();

    else if (strcmp(base_command, "shutdown") == 0) {
        outw(0x604, 0x2000);  // Temporary solution for QEMU
        error_msg("Incorrect shutdown command");
    }

    else if (strcmp(base_command, "crash") == 0)
        asm("int $18");

    else if (strcmp(base_command, "help") == 0)
        cmd_help();

    else {
        kprint("Unknown command: ");
        kprint(base_command);
    }

    strwipe(command_buffer);
    kprint("\n\n");
    kprint((char *)CLI_PREFIX);
}