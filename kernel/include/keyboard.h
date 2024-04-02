#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>
#include <stddef.h>

#define KEYBOARD_BUFFER_MAXSIZE 64

enum KB_SPECIAL_KEY {
    KEY_NULL = 0,
    
    // Command processing
    KEY_ESCAPE = 0x01,
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
    KEY_ALT = 0x38,

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

typedef uint8_t scancode_t;

typedef struct {
    size_t front_pos;
    size_t back_pos;
    scancode_t buffer[KEYBOARD_BUFFER_MAXSIZE];
} keyboard_t;


void keyboard_init();

// Conversion functions
bool scancode_to_char(const scancode_t sc, char *c);
bool scancode_is_special(const scancode_t sc);

// Keyboard buffer functions
bool keyboard_performed_event();
scancode_t *keyboard_buffer_get();
void keyboard_buffer_flush();
bool keyboard_buffer_empty();
bool keyboard_buffer_full();
bool keyboard_buffer_next(scancode_t *sc);
bool keyboard_buffer_last(scancode_t *sc);
bool keyboard_buffer_pop();

#endif