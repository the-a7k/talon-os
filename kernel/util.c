#include "string.h"


void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}


void int_to_ascii(int num, char str[]) {
    short sign = 1;
    if (num < 0) {
        sign = 0;
        num = num * (-1);
    }

    int iteration = 0;
    do {
        int digit = num % 10;
        num = num / 10;
        str[iteration++] = '0' + digit;
    }
    while (num != 0);

    if (sign == 0) {
       str[iteration++] = '-'; 
    }

    str[iteration++] = '\0';
    str_reverse(str);
}