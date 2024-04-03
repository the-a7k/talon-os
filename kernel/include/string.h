#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdbool.h>

void itoa(int num, char *str);
void htoa(int num, char *str);

// String functions
size_t strlen(const char *str);
void strrev(char *str);
int strcmp(const char *first, const char *second);
void strcat(char *dest, const char *src);
void strwipe(char *str);
void strpop(char *str);
void strtolower(char *str);
void strtoupper(char *str);
char *strtok(char *str, const char *delim);

// Char functions
void charcat(char *str, const char to_add);
void chartolower(char *c);
void chartoupper(char *c);

#endif