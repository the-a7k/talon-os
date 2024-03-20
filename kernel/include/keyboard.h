#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>

#define KEYBOARD_BUFFER_LENGTH 128

enum KB_SPECIAL_KEY {
    KEY_NULL = 0,
    
    KEY_ENTER = 0x1C,
    KEY_TAB = 0x0F,
    KEY_BACKSPACE = 0x0E,

    KEY_LSHIFT = 0x2A,
    KEY_LSHIFT_UP = 0xAA,

    KEY_RSHIFT = 0x36,
    KEY_RSHIFT_UP = 0xB6,

    KEY_LCTRL= 0x1D,
    KEY_RCTRL = 0xE01D,

    KEY_LALT = 0x38,
    KEY_CAPSLOCK = 0x3A,

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
    char buffer[KEYBOARD_BUFFER_LENGTH];
    bool buffer_full;
    char last_key;
    bool is_special;
    enum KB_SPECIAL_KEY special_key;
} keyboard_t;


void keyboard_init();
char *keyboard_get_buffer();
char keyboard_get_key();
bool keyboard_handle_action();
keyboard_t *keyboard_get();

#endif