#include <stdint.h>
#include "../include/string.h"
#include "../include/mem.h"


void itoa(int num, char *str) {
    bool is_negative = false;
    if (num < 0) {
        is_negative = true;
        num = num * (-1);
    }

    size_t i = 0;
    do {
        int digit = num % 10;
        num = num / 10;
        str[i++] = '0' + digit;
    } while (num != 0);

    if (is_negative)
       str[i++] = '-';

    str[i++] = '\0';
    strrev(str);
}


void htoa(int num, char *str) {
    strcat(str, "0x");
    uint8_t digits = 0;
    int32_t current;

    for (size_t i = 28; i > 0; i -= 4) {
        current = (num >> i) & 0xF;
        if (current == 0 && digits == 0) 
            continue;
        digits = 1;
        if (current >= 0xA) 
            charcat(str, current - 0xA + 'a');
        else 
            charcat(str, current + '0');
    }

    current = num & 0xF;
    if (current >= 0xA) 
        charcat(str, current - 0xA + 'a');
    else 
        charcat(str, current + '0');
}


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
        *c += 32;  // 32 = ASCII lower/upper offset
}


void chartoupper(char *c) {
    if (*c >= 'a' && *c <= 'z')
       *c -= 32;
}