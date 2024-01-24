#include "../drivers/tty.h"
#include "util.h"


void main() {
    clear_screen();

    for (int i = 0; i < 25; i++) {
        char str[25];
        int_to_ascii(i, str);
        kprint(str);
        newline();
    }

    kprint("\nScroll test: \tshifting line 1");
    kprint_color("\nScroll test: \tshifting line 2", BLUE, WHITE);

}

