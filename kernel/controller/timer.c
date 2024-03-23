#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/ports.h"

static volatile uint32_t tick = 0;

uint32_t timer_get_tick() {
    return tick;
}


uint32_t timer_calc_ms(uint32_t tick) {
    float freq_divisor = PIT_FREQ / PIT_FREQ_DIVISOR / 1000.0;
    return (uint32_t)(tick / freq_divisor);
}


uint32_t timer_calc_sec(uint32_t tick) {
    float freq_divisor = PIT_FREQ / PIT_FREQ_DIVISOR;
    return (uint32_t)(tick / freq_divisor);
}


uint32_t timer_calc_tick(uint32_t ms) {
    float freq_divisor = PIT_FREQ / PIT_FREQ_DIVISOR;
    return (uint32_t)(ms * freq_divisor / 1000);
}


void cpu_sleep(uint32_t ms) {
    volatile uint32_t tick_sleep_end = timer_calc_tick(
        timer_calc_ms(timer_get_tick()) + ms
    );
    do {
        asm volatile("hlt");
    } while(timer_get_tick() < tick_sleep_end);
}


static void timer_callback(registers_t *reg) {
    tick++;
}


void timer_init() {
    interrupt_handler_install(IRQ0, timer_callback);

    uint8_t low = (uint8_t)(PIT_FREQ_DIVISOR & 0xFF);
    uint8_t high = (uint8_t)((PIT_FREQ_DIVISOR >> 8) & 0xFF);

    outb(PIT_COMMAND, 0x36);  // Command port
    outb(PIT_CHANNEL_0, low);
    outb(PIT_CHANNEL_0, high);
}