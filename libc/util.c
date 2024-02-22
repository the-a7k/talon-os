#include "util.h"
#include "string.h"


void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}


void itoa(int num, char str[]) {
    bool is_negative = false;
    if (num < 0) {
        is_negative = true;
        num = num * (-1);
    }

    int i = 0;
    do {
        int digit = num % 10;
        num = num / 10;
        str[i++] = '0' + digit;
    }
    while (num != 0);

    if (is_negative) {
       str[i++] = '-'; 
    }

    str[i++] = '\0';
    strrev(str);
}