#include "keyboard.h"
#include "tty.h"
#include "ports.h"
#include "../controller/isr.h"
#include "../libc/string.h"

#define KEYBOARD_BUFFER_LENGTH 256

static uint8_t key_buffer[KEYBOARD_BUFFER_LENGTH];
static bool uppercase_flag = false;
static bool text_region_limit = false;


uint8_t kb_scancode_keys[] = {
    // Keyboard characters indexed by scancode
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',   // 1234
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',               // QWERTY
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,              // ASDF
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,                      // ZXCV
    '*',
    0,
    ' ',  // Space
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Function keys
    '7', '8', '9', '-',  // Keypad
    '4', '5', '6', '+', 
    '1', '2', '3',
    '0', '.',
};


static void keyboard_handle_key(uint8_t key) {
    if (key == '\b') {
        size_t tr_active = tr_get_active();
        uint8_t current_col = calc_col(get_cursor_pos());
        uint8_t current_row = calc_row(get_cursor_pos());

        if (tr_get_dest_col(tr_active) == current_col && tr_get_dest_row(tr_active) == current_row) {
            if (text_region_limit) {
                cursor_retreat();
                clear_cell_cursor();
                text_region_limit = false;
            }
            else {
                clear_cell(current_col, current_row);
                text_region_limit = true;
            }
        }
        else {
            cursor_retreat();
            clear_cell_cursor();
        }
        strpop(key_buffer);
    }
    else if (key == '\n') {
        newline();
        // TODO: Handle user input
    }
    else {
        text_region_limit = false;
        char output[2] = {key, '\0'};
        if (uppercase_flag) {
            strtoupper(output);
        }
        strcat(key_buffer, output);
        kprint(output);
    }
}


static void keyboard_handle_function(uint8_t scancode) {
    switch (scancode) {
        case(0x3A):  // Caps-lock
        case(0x2A):  // L-shift keydown
        case(0xAA):  // L-shift keyup
        case(0x36):  // R-shift keydown
        case(0xB6):  // R-shift keyup
            uppercase_flag = !uppercase_flag;
            break;
    }
}


static void keyboard_callback(registers_t reg) {
    uint8_t scancode = inb(0x60);
    uint8_t key = kb_scancode_keys[scancode];

    if (key) {
        keyboard_handle_key(key);
    }
    else {
        keyboard_handle_function(scancode);
    }
}


void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback);
}