#include <stddef.h>
#include <stdint.h>
#include "keyboard.h"
#include "tty.h"
#include "ports.h"
#include "../controller/isr.h"
#include "../drivers/ports.h"
#include "../../libc/string.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SC_LAST_HANDLE 57
#define KEYBOARD_BUFFER_LENGTH 128

static char key_buffer[KEYBOARD_BUFFER_LENGTH];

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static void keyboard_callback(registers_t reg) {
    uint8_t scancode = inb(0x60);
    
    if (scancode > SC_LAST_HANDLE) {
        return;
    }
    else if (scancode == BACKSPACE) {
        cursor_retreat();
        clear_cell(
            calc_col(get_cursor_pos()),
            calc_row(get_cursor_pos())
        );
        strpop(key_buffer);
    }
    else if (scancode == ENTER) {
        newline();
        // TODO: Handle user input
    }
    else if (strlen(key_buffer) > KEYBOARD_BUFFER_LENGTH) {
        return;
    }
    else {
        char key = sc_ascii[(uint8_t)scancode];
        if (key == '?') {
            return;
        }
        char output[2] = {key, '\0'};
        charcat(key_buffer, key);
        kprint(output);
    }
}


void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}

