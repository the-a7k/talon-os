#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void init_keyboard();
void uppercase_check(uint8_t scancode, bool *flag);

#endif