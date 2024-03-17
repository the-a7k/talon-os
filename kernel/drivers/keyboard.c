#include "keyboard.h"
#include "tty.h"
#include "ports.h"
#include "command.h"
#include "../controller/isr.h"
#include "../libc/string.h"

#define KEYBOARD_BUFFER_LENGTH 128

static char key_buffer[KEYBOARD_BUFFER_LENGTH];
static bool uppercase_flag = false;


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
    if (strlen(key_buffer) >= KEYBOARD_BUFFER_LENGTH -1 && key != '\b' && key != '\n')
        return;  // Buffer overflow prevention

    switch(key) {
        case('\b'):
            if (strlen(key_buffer) != 0) {
                cursor_retreat();
                clear_cell_cursor();
                strpop(key_buffer);
            }
            break;

        case('\n'):
            newline();
            if (strlen(key_buffer) > 0) {
                command_handle(key_buffer);
                strwipe(key_buffer);
                kprint("\n\n");
                kprint((char*)CLI_PREFIX);
            }
            break;

        default:
            if (uppercase_flag)
                chartoupper(key);
            charcat(key_buffer, key);
            kcprint(key);
            break;
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


static void keyboard_callback(registers_t *reg) {
    uint8_t scancode = inb(0x60);
    uint8_t key = kb_scancode_keys[scancode];
    if (key)
        keyboard_handle_key(key);
    else
        keyboard_handle_function(scancode);     
}


void init_keyboard() {
    interrupt_handler_install(IRQ1, keyboard_callback);
}