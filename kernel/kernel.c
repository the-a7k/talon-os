#include "drivers/tty.h"
#include "drivers/keyboard.h"
#include "controller/isr.h"
#include "controller/idt.h"
#include "controller/timer.h"
#include "../libc/util.h"
#include "../libc/string.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

void main() {
    isr_setup();
    clear_screen();

    move_cursor(100,50);        // Illegal position
    asm volatile("int $18");    // Unknown interrupt
    asm volatile("sti");        // IRQ initialization

    kprint("\nDebug mode commands: \n\tCTRL - Get cursor pos\n");

    init_timer(1);
    init_keyboard();
}

