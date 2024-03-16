#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char *str);
void strrev(char *str);
bool strcomp(const char *first, const char *second);
void strcat(char *dest, const char *source);
void charcat(char *str, char to_add);
void strwipe(char *str);
void strpop(char *str);
void strtolower(char *str);
void strtoupper(char *str);
void chartolower(char character);
void chartoupper(char character);

#endif