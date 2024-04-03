#include <stdint.h>
#include "../include/keyboard.h"
#include "../include/ports.h"
#include "../include/isr.h"


static bool keyboard_event_flag = false;      // Triggers on every keyboard interrupt
keyboard_t keyboard_main = { 0 };             // Keyboard key buffer controller


static const char keyboard_scancode_keys[255] = {
    // Keyboard map of basic characters indexed by scancode
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

static const char keyboard_scancode_keys_shift[255] = {
    // Keyboard map of shiftable characters
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
};


bool scancode_to_char(const scancode_t sc, char *c) {
    if (!scancode_is_special(sc)) {
        *c = keyboard_scancode_keys[sc];
        return true;
    }
    return false;
}


bool scancode_to_shifted(const scancode_t sc, char *c) {
    if (scancode_can_shift(sc)) {
        *c = keyboard_scancode_keys_shift[sc];
        return true;
    }
    return false;
}


bool scancode_is_special(const scancode_t sc) {
    // Returns false if scancode is a character
    return !keyboard_scancode_keys[sc];
}


bool scancode_can_shift(const scancode_t sc) {
    return keyboard_scancode_keys_shift[sc];
}


bool keyboard_performed_event() {
    if (keyboard_event_flag) {
        keyboard_event_flag = false;
        return true;
    }
    return false;
}


scancode_t *keyboard_buffer_get() {
    return keyboard_main.buffer;
}


void keyboard_buffer_flush() {
    // Works as initializer or destoyer, unhandled scancodes will stay in buffer 
    keyboard_main.front_pos = keyboard_main.back_pos = 0;
}


bool keyboard_buffer_empty() {
    return keyboard_main.front_pos == keyboard_main.back_pos;
}


bool keyboard_buffer_full() {
    return (keyboard_main.back_pos + 1) % KEYBOARD_BUFFER_MAXSIZE == keyboard_main.front_pos;
}


static bool keyboard_buffer_push(const scancode_t sc) {
    // Needs to be static, push used only by keyboard interrupt itself
    if (keyboard_buffer_full(keyboard_main))
        return false;

    const size_t next_pos = (keyboard_main.back_pos + 1) % KEYBOARD_BUFFER_MAXSIZE;
    keyboard_main.buffer[keyboard_main.back_pos] = sc;
    keyboard_main.back_pos = next_pos;
    return true;
}


bool keyboard_buffer_pop() {
    if (!keyboard_buffer_empty(keyboard_main)) {
        keyboard_main.front_pos = (keyboard_main.front_pos + 1) % KEYBOARD_BUFFER_MAXSIZE;
        return true;
    }
    return false;
}



bool keyboard_buffer_next(scancode_t *sc) {
    // Get next (oldest) value and save it
    if (!keyboard_buffer_empty(keyboard_main) && sc) {
        *sc = keyboard_main.buffer[keyboard_main.front_pos];
        return true;
    }
    return false;
}


bool keyboard_buffer_last(scancode_t *sc) {
    // Get last (latest) value and save it
    if (!sc || keyboard_buffer_empty(keyboard_main))
        return false;

    const size_t last_pos = (keyboard_main.back_pos) ? (keyboard_main.back_pos - 1) : (KEYBOARD_BUFFER_MAXSIZE - 1);
    *sc = keyboard_main.buffer[last_pos];
    return true;
}


static void keyboard_callback(registers_t *reg) {
    scancode_t sc = inb(0x60);  // Scancode location
    keyboard_buffer_push(sc);
    keyboard_event_flag = true;
}


void keyboard_init() {
    interrupt_handler_install(IRQ1, keyboard_callback);
    keyboard_buffer_flush(&keyboard_main);
}