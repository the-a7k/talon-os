#include <stdint.h>
#include "../include/keyboard.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/isr.h"
#include "../include/string.h"


static bool kb_uppercase_flag = false;  // Triggers if last key was uppercase handler  
static bool kb_event_flag = false;      // Triggers on any key / function event
static keyboard_t kb_event = {
    .char_buffer = { 0 },
    .special_buffer = { 0 },
    .buffer_full = false
};


static uint8_t kb_scancode_keys[] = {
    // Keyboard map of writeable characters indexed by scancode
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
    kb_event_flag = true;

    if (kb_uppercase_flag)
        chartoupper(&key);

    if (!queue_push(&kb_event.char_buffer, key) ||
        queue_is_full(&kb_event.special_buffer))
            kb_event.buffer_full = true;
    else
        kb_event.buffer_full = false;
}


static void keyboard_handle_function(uint8_t scancode) {
    switch (scancode) {
        case(KEY_CAPSLOCK):
        case(KEY_LSHIFT):
        case(KEY_LSHIFT_UP):
        case(KEY_RSHIFT):
        case(KEY_RSHIFT_UP):
            kb_uppercase_flag = !kb_uppercase_flag;
            kb_event_flag = true;
            break;
        
        case(KEY_ENTER):
        case(KEY_BACKSPACE):
            if (!queue_push(&kb_event.char_buffer, 0) ||
                !queue_push(&kb_event.special_buffer, scancode))
                    kb_event.buffer_full = true;
            kb_event_flag = true;
            break;
    }
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
    queue_init(&kb_event.char_buffer);
    queue_init(&kb_event.special_buffer);
}