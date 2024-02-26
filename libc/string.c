#include <stdint.h>
#include "string.h"


size_t strlen(const char *str) {
    size_t size = 0;
    while (str[size] != '\0') {
        size++;
    }
    return size;
}


void strrev(char *str) {
    int current = 0;
    for (int i = strlen(str)-1; i >= current; i--) {
        char current_char = str[current];
        str[current++] = str[i];
        str[i] = current_char;
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