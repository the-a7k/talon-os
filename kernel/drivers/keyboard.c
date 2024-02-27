#include "keyboard.h"
#include "tty.h"
#include "ports.h"
#include "../controller/isr.h"
#include "../../libc/string.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SC_LAST_HANDLE 57
#define KEYBOARD_BUFFER_LENGTH 256

static char key_buffer[KEYBOARD_BUFFER_LENGTH];
static bool uppercase_flag = false;
const char sc_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', 
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?', '?', 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
};


static void keyboard_callback(registers_t reg) {
    uint8_t scancode = inb(0x60);
    uppercase_check(scancode, &uppercase_flag);

    if (scancode == BACKSPACE) {
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
    else if (strlen(key_buffer) > KEYBOARD_BUFFER_LENGTH || scancode > SC_LAST_HANDLE) {
        return;
    }
    else {
        // TODO: Improve this
        char key = sc_ascii[(uint8_t)scancode];
        if (key == '?') {
            return;
        }
        char output[2] = {key, '\0'};
        charcat(key_buffer, key);
        if (uppercase_flag) {
            strtoupper(output);
        }
        kprint(output);
    }
}


void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}


void uppercase_check(uint8_t scancode, bool *flag) {
    // Caps-Lock: 0x3A (0xBA release)
    // L-Shift:   0x2A (0xAA release) 
    // R-Shift:   0x36 (0xB6 release)

    if (scancode == 0x3A || 
    scancode == 0x2A || 
    scancode == 0x36 || 
    scancode == 0xAA || 
    scancode == 0xB6) {
        *flag = !*flag;
    }
}
