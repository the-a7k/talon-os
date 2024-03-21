#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>
#include "../include/keyboard.h"

void command_key_handler(keyboard_t *ctx);
void command_execute(const char *command);

#endif