#include <stddef.h>
#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/timer.h"
#include "../include/buzzer.h"
#include "../include/string.h"

#define COMMAND_MAXSIZE 128
#define COMMAND_HISTORY_MAXSIZE 16

static char command_buffer[COMMAND_MAXSIZE];
static char *command_history[COMMAND_HISTORY_MAXSIZE];
static bool uppercase_flag = false;


static void cmd_help() {
    kprint_color("List of available commands:\n", TTY_BLACK, TTY_LIGHT_GREEN);
    kprint("\ttime      (show uptime in seconds and ticks)\n");
    kprint("\tsleep     (beep and pause the CPU for 3 seconds)\n");
    kprint("\tcls       (clear screen)\n");
    kprint("\tcrash     (crash the system with ISR handler)\n");
    kprint("\tshutdown  (shut down VM machine)\n");
    kprint("\treboot    (restart VM machine)\n");
    kprint("\tdebug     (enter keyboard debug mode)\n");
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
    buzzer_play(500,100);
    buzzer_play(800,100);
    cpu_sleep(3000-200);
    kprint_color("Resumed!", TTY_BLACK, TTY_LIGHT_MAGENTA);
    buzzer_play(800,100);
    buzzer_play(500,100);
}


static void cmd_reboot() {
    kprint_color("Restarting...", TTY_BLACK, TTY_LIGHT_BLUE);
    cpu_sleep(200);
    asm("cli");
    outb(0x64, 0xFE);  // CPU reboot
}


static void cmd_debug() {
    kprint_color("Keyboard debug mode (press ESC to abort)\n", TTY_BLACK, TTY_CYAN);
    kprint_color("Flushing key buffer...\n\n", TTY_BLACK, TTY_BLUE);
    keyboard_buffer_flush();
    bool kb_debug_stop_flag = false;

    while (!kb_debug_stop_flag) {
        if (keyboard_performed_event()) {
            scancode_t sc = 0;

            while (!keyboard_buffer_empty()) {
                if (!keyboard_buffer_next(&sc)) {
                    kprint_color("Buffer error (abort)\n", TTY_BLACK, TTY_RED);
                    kb_debug_stop_flag = true;
                    break;
                }
                else if (sc == KEY_ESCAPE) {
                    kb_debug_stop_flag = true;
                    break;
                }
                keyboard_buffer_pop();

                kprint("INT: ");
                kprintint(sc);

                kprint("\tHEX: ");
                kprinthex(sc);

                kprint("\tCHAR: ");
                char sc_temp = 0;
                scancode_to_char(sc, &sc_temp);
                kputchar(sc_temp);

                newline();
            }
            newline();
        }
    }
    kprint_color("Flushing key buffer...\n", TTY_BLACK, TTY_BLUE);
    keyboard_buffer_flush();
    kprint_color("Debug end", TTY_BLACK, TTY_CYAN);
}


static void command_backspace() {
    if (strlen(command_buffer)) {
        cursor_retreat();
        clear_cell_cursor();
        strpop(command_buffer);
    }
}


static void shell_print_prefix() {
    kprint((char *)CLI_PREFIX);
}


static void shell_key_handler_overflow() {
    scancode_t process_command = 0;
    if (keyboard_buffer_next(&process_command)) {
        if (process_command == KEY_ENTER)
            shell_execute(command_buffer);
        else if (process_command == KEY_BACKSPACE) {
            command_backspace();
        }
    }
    keyboard_buffer_flush();  // Keyboard buffer reset
}


void shell_key_handler() {
    if (keyboard_buffer_empty())
        return;
    
    scancode_t current_key = 0;
    char converted = 0;

    while (keyboard_buffer_next(&current_key)) {
        if (strlen(command_buffer) == COMMAND_MAXSIZE - 1) {
            shell_key_handler_overflow();
            return;
        }

        else if (scancode_to_char(current_key, &converted)) {
            if (uppercase_flag)
                chartoupper(&converted);
            kputchar(converted);
            charcat(command_buffer, converted);
        }

        else if (scancode_is_special(current_key)) {
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
                        shell_execute(command_buffer);
                    else {
                        newline();
                        shell_print_prefix();
                    }
                    break;
                case(KEY_TAB):
                    tab();
                    break;
            }
        }
        keyboard_buffer_pop();
    }
}


void shell_execute(const char *command) {
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

    else if (strcmp(base_command, "cls") == 0)
        clear_screen();

    else if (strcmp(base_command, "sleep") == 0)
        cmd_sleep();

    else if (strcmp(base_command, "shutdown") == 0) {
        outw(0x604, 0x2000);  // Temporary solution for QEMU
        error_msg("Incorrect shutdown command");
    }
    else if (strcmp(base_command, "reboot") == 0)
        cmd_reboot();

    else if (strcmp(base_command, "crash") == 0)
        asm("int $18");

    else if (strcmp(base_command, "debug") == 0)
        cmd_debug();

    else if (strcmp(base_command, "help") == 0)
        cmd_help();

    else {
        kprint("Unknown command: ");
        kprint(base_command);
    }

    strwipe(command_buffer);
    kprint("\n\n");
    shell_print_prefix();
}


void shell_setup() {
    clear_screen();
    kprint_color("Welcome to talonOS! Type 'help' to get started.\n", TTY_BLACK, TTY_LIGHT_BLUE);
    shell_print_prefix();
}