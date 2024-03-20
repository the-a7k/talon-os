#include <stdint.h>
#include <stddef.h>
#include "../include/keyboard.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/isr.h"
#include "../include/string.h"


static bool kb_uppercase_flag = false;  // Triggers if last key was uppercase action  
static bool kb_action_flag = false;     // Triggers on any key
keyboard_t *kb_event;                   // Contains information about key event


uint8_t kb_scancode_keys[] = {
    // Keyboard characters indexed by scancode
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,      // 1234
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,               // QWERTY
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,           // ASDF
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,                   // ZXCV
    '*', 0, ' ',                                                                 // Space
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                       // Function keys
    '7', '8', '9', '-',                                                          // Keypad
    '4', '5', '6', '+', 
    '1', '2', '3',
    '0', '.',
};


static void keyboard_handle_key(uint8_t key) {
    kb_event->is_special = false;
    kb_event->special_key = KEY_NULL;
    kb_action_flag = true;

    if (strlen(kb_event->buffer) >= KEYBOARD_BUFFER_LENGTH - 1) {
        kb_event->buffer_full = true;
        kb_event->last_key = 0;
        return;
    }

    if (kb_uppercase_flag)
        chartoupper(&key);

    charcat(kb_event->buffer, key);
    kb_event->buffer_full = false;
    kb_event->last_key = key;
}


static void keyboard_handle_function(uint8_t scancode) {
    switch (scancode) {
        case(KEY_CAPSLOCK):
        case(KEY_LSHIFT):
        case(KEY_LSHIFT_UP):
        case(KEY_RSHIFT):
        case(KEY_RSHIFT_UP):
            kb_uppercase_flag = !kb_uppercase_flag;
            kb_action_flag = true;
            break;
        
        case(KEY_ENTER):
        case(KEY_BACKSPACE):
            kb_event->is_special = true;
            kb_event->special_key = scancode;
            kb_action_flag = true;
            break;
    }
}


bool keyboard_handle_action() {
    if (kb_action_flag) {
        kb_action_flag = false;
        return true;
    }
    return false;
}


keyboard_t *keyboard_get() {
    return kb_event;
}


static void keyboard_callback(registers_t *reg) {
    uint8_t scancode = inb(0x60);
    uint8_t key = kb_scancode_keys[scancode];

    if (key)
        keyboard_handle_key(key);
    else
       keyboard_handle_function(scancode);
}


void keyboard_init() {
    interrupt_handler_install(IRQ1, keyboard_callback);
    strwipe(kb_event->buffer);
    kb_event->is_special = false;
    kb_event->special_key = KEY_NULL;
    kb_event->buffer_full = false;
    kb_event->last_key = 0;
}