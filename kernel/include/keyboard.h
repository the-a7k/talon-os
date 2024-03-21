#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>
#include <stddef.h>
#include "../include/queue.h"

enum KB_SPECIAL_KEY {
    KEY_NULL = 0,
    
    // Command processing
    KEY_ENTER = 0x1C,
    KEY_TAB = 0x0F,
    KEY_BACKSPACE = 0x0E,

    // Uppercase text handling
    KEY_CAPSLOCK = 0x3A,
    KEY_LSHIFT = 0x2A,
    KEY_LSHIFT_UP = 0xAA,
    KEY_RSHIFT = 0x36,
    KEY_RSHIFT_UP = 0xB6,

    KEY_LCTRL= 0x1D,
    KEY_RCTRL = 0xE01D,
    KEY_ALT = 0x38,  // Left Alt + AltGr

    // Function keys
    KEY_F1 = 0x3B,
    KEY_F2 = 0x3C,
    KEY_F3 = 0x3D,
    KEY_F4 = 0x3E,
    KEY_F5 = 0x3F,
    KEY_F6 = 0x40,
    KEY_F7 = 0x41,
    KEY_F8 = 0x42,
    KEY_F9 = 0x43,
    KEY_F10 = 0x44,
    KEY_F11 = 0x57,
    KEY_F12 = 0x58,
};


typedef struct {
    queue_t char_buffer;        // Buffer with ASCII characters
    queue_t special_buffer;    // Buffer with special
    bool buffer_full;
} keyboard_t;


void keyboard_init();
bool keyboard_performed_event();
keyboard_t *keyboard_get();

#endif