#include "drivers/tty.h"
#include "../libc/util.h"
#include "controller/isr.h"
#include "controller/idt.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

void main() {
    isr_setup();
    clear_screen();

    for (size_t i = 0; i < 8; i++) {
        char str[25];
        itoa(i, str);
        kprint("Line: ");
        kprint(str);
        newline();
    }


    move_cursor(100,50);    // Illegal position
    kprint("User error.\n");

    asm volatile("int $15");    // Unknown interrupt
    kprint("Computer interrupt.");
}

