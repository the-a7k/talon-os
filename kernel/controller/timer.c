#include "timer.h"
#include "isr.h"
#include "../drivers/ports.h"
#include "../../libc/util.h"

#define PIT_FREQ 1193180

uint32_t tick = 0;


static void timer_callback(registers_t reg) {
    tick++;
}


void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = PIT_FREQ / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x43, 0x36); // Command port
    outb(0x40, low);
    outb(0x40, high);
}


uint32_t get_tick() {
    return tick;
}