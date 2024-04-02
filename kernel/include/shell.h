#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdint.h>
#include "../include/keyboard.h"

#define CLI_PREFIX "kernel>"

void shell_key_handler();
void shell_execute(const char *command);
void shell_setup();

#endif