#include "../include/ctype.h"

bool islower(const char c) {
    return c >= 'a' && c <= 'z';
}
    

bool isupper(const char c) {
    return c >= 'A' && c <= 'Z';
}
    

bool isdigit(const char c) {
    return c >= '0' && c <= '9';
}


bool isalpha(const char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}