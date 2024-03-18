#include <stdbool.h>
#include "utility.h"
#include "string.h"


void itoa(int num, char str[]) {
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