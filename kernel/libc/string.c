#include <stdint.h>
#include "../include/string.h"
#include "../include/mem.h"

#define ASCII_CAPITAL_OFFSET 32


// String functions
size_t strlen(const char *str) {
    size_t size = 0;
    while (str[size] != '\0')
        size++;
    return size;
}


void strrev(char *str) {
    size_t end_pointer = strlen(str)-1;
    for (size_t i = 0; i < end_pointer; i++) {
        char current_char = str[i];
        str[i] = str[end_pointer];
        str[end_pointer--] = current_char;
    }
}


int strcmp(const char *first, const char *second) {
    size_t i;
    for (i = 0; first[i] == second[i]; i++) {
        if (first[i] == '\0') 
            return 0;
    }
    return first[i] - second[i];
}


void strcat(char *dest, const char *src) {
    for (size_t i = 0; i < strlen(src); i++)
        charcat(dest, src[i]);
}


void strwipe(char *str) {
    memset(str, 0, strlen(str));
    str[0] = '\0';
}


void strpop(char *str) {
    size_t length = strlen(str);
    if (length == 0)
        return;
    str[length-1] = '\0';
}


void strtolower(char *str) {
    for (size_t i = 0; i < strlen(str); i++)
        chartolower(&str[i]);
}


void strtoupper(char *str) {
    for (size_t i = 0; i < strlen(str); i++)
        chartoupper(&str[i]);
}


// Char functions
void charcat(char *str, const char to_add) {
    size_t length = strlen(str);
    str[length] = to_add;
    str[length+1] = '\0';
}


void chartolower(char *c) {
    if (*c >= 'A' && *c <= 'Z') 
        *c += ASCII_CAPITAL_OFFSET;
}


void chartoupper(char *c) {
    if (*c >= 'a' && *c <= 'z')
       *c -= ASCII_CAPITAL_OFFSET;
}