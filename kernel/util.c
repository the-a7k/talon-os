#include "util.h"
#include "string.h"


void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}


void itoa(int num, char str[]) {
    bool sign = 1;
    if (num < 0) {
        sign = 0;
        num = num * (-1);
    }

    int i = 0;
    do {
        int digit = num % 10;
        num = num / 10;
        str[i++] = '0' + digit;
    }
    while (num != 0);

    if (sign == 0) {
       str[i++] = '-'; 
    }

    str[i++] = '\0';
    strrev(str);
}