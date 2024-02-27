#include "drivers/tty.h"
#include "controller/isr.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

void main() {
    isr_setup();
    irq_setup();
    clear_screen();

    move_cursor(100,50);        // Illegal position error
}

