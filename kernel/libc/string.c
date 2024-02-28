#include <stdint.h>
#include "string.h"

#define ASCII_CAPITAL_OFFSET 32

size_t strlen(const char *str) {
    size_t size = 0;
    while (str[size] != '\0') {
        size++;
    }
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


void strcat(char *dest, const char *source) {
    for (size_t i = 0; i < strlen(source); i++) {
        charcat(dest, source[i]);
    }
}


void charcat(char *str, const char to_add) {
    size_t length = strlen(str);
    str[length] = to_add;
    str[length+1] = '\0';
}


void strpop(char *str) {
    size_t length = strlen(str);
    str[length-1] = '\0';
}


void strtolower(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += ASCII_CAPITAL_OFFSET; 
        }
    }
}


void strtoupper(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= ASCII_CAPITAL_OFFSET; 
        }
    }
}