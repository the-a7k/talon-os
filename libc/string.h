#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

size_t strlen(const char *str);
void strrev(char *str);
void strcat(char *dest, const char *source);
void charcat(char *str, char to_add);
void strpop(char *str);
void strtolower(char *str);
void strtoupper(char *str);

#endif