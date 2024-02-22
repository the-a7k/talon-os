#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

void init_keyboard();
void handle_key(uint8_t scancode);

#endif