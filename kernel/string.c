int str_len(char* str) {
    int size = 0;
    while (str[size] != '\0') {
        size++;
    }
    return size;
}


void str_reverse(char str[]) {
    int current = 0;
    for (int i = str_len(str)-1; i >= current; i--) {
        char current_char = str[current];
        str[current++] = str[i];
        str[i] = current_char;
    }
}