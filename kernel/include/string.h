#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdbool.h>

// String functions
size_t strlen(const char *str);
void strrev(char *str);
int strcmp(const char *first, const char *second);
void strcat(char *dest, const char *src);
void strwipe(char *str);
void strpop(char *str);
void strtolower(char *str);
void strtoupper(char *str);

// Char functions
void charcat(char *str, const char to_add);
void chartolower(char *c);
void chartoupper(char *c);

#endif