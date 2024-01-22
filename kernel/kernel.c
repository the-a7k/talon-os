#include "../drivers/screen.h"
#include "util.h"


void main() {
    clear_screen();

    for (int i = 0; i < 25; i++) {
        char str[10];
        int_to_ascii(i, str);
        print_at(str, 0, i);
    }
    
    print_cursor("\nHey\nHere\nHello\nBye\nGoodbye");
    clear_row(10);
    print_cursor("!");
}

