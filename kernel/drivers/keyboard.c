#include <stdint.h>
#include "../include/keyboard.h"
#include "../include/ports.h"
#include "../include/isr.h"


static bool kb_event_flag = false;      // Triggers on every successful key or function event
static keyboard_t kb_event = {          // Struct of currently unhandled keyboard events
    .key_buffer = { 0 },                // Buffer with scancodes
    .buffer_full = false                // Flag indicating full buffer
};


static uint8_t kb_scancode_keys[] = {
    // Keyboard map of basic writeable characters indexed by scancode
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,    // 1234
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,             // QWERTY
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,         // ASDF
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,                 // ZXCV
    '*', 0, ' ',                                                               // Space
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                     // Function keys
    '7', '8', '9', '-',                                                        // Keypad
    '4', '5', '6', '+', 
    '1', '2', '3',
    '0', '.',
};


bool keyboard_performed_event() {
    if (kb_event_flag) {
        kb_event_flag = false;
        return true;
    }
    return false;
}


keyboard_t *keyboard_get() {
    return &kb_event;
}


bool keyboard_to_char(uint8_t scancode, char *c) {
    if (!keyboard_is_special(scancode)) {
        *c = kb_scancode_keys[scancode];
        return true;
    }
    return false;
}


bool keyboard_is_special(uint8_t scancode) {
    return !kb_scancode_keys[scancode];
}


static void keyboard_callback(registers_t *reg) {
    uint8_t scancode = inb(0x60);
    
    if (!queue_push(&kb_event.key_buffer, scancode))
        kb_event.buffer_full = true;
    else
        kb_event.buffer_full = false;
    kb_event_flag = true;
}


void keyboard_init() {
    interrupt_handler_install(IRQ1, keyboard_callback);
    queue_init(&kb_event.key_buffer);
}