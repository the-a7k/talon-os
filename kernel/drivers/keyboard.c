#include <stdint.h>
#include "../include/keyboard.h"
#include "../include/ports.h"
#include "../include/isr.h"
#include "../include/string.h"


static bool kb_event_flag = false;      // Triggers on every successful key or function event
static keyboard_t kb_event = {          // Struct of currently unhandled keyboard events
    .key_buffer = { 0 },                // Buffer with lowercase ASCII chars (on key-press) or 0 (zero indicates to check the special buffer)
    .special_buffer = { 0 },            // Buffer with scancodes of special functions or key-releases
    .buffer_full = false                // Flag indicating full buffer(s)
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


static void keyboard_handle_key(uint8_t key) {
    if (!queue_push(&kb_event.key_buffer, key) ||
        queue_is_full(&kb_event.special_buffer))
            kb_event.buffer_full = true;
    else
        kb_event.buffer_full = false;
    kb_event_flag = true;
}


static void keyboard_handle_function(uint8_t scancode) {
    if (!queue_push(&kb_event.key_buffer, 0) ||
        !queue_push(&kb_event.special_buffer, scancode))
            kb_event.buffer_full = true;
    kb_event_flag = true;
}


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
    queue_init(&kb_event.key_buffer);
    queue_init(&kb_event.special_buffer);
}