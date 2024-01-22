#include "../drivers/screen.h"
#include "util.h"


void main() {
    clear_screen();

    for (int i = 0; i < 25; i++) {
        char str[25];
        int_to_ascii(i, str);
        print_at(str, 0, i);
    }

    print_cursor("\nScroll test - shifting line 0");
    print_cursor("\nScroll test - shifting line 1");

    clear_row(0);
    

}

