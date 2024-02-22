#include "string.h"

size_t strlen(char *str) {
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