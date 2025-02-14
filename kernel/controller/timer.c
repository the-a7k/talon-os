#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/ports.h"

static volatile uint32_t tick = 0;
static const float freq_period = PIT_FREQ / PIT_FREQ_DIVISOR;

static void timer_callback(registers_t *reg) {
    tick++;
}


void timer_init() {
    // Initialize PIT timer (with buzzer on PIT 02)
    interrupt_handler_install(IRQ0, timer_callback);

    uint8_t low = (uint8_t)(PIT_FREQ_DIVISOR & 0xFF);
    uint8_t high = (uint8_t)((PIT_FREQ_DIVISOR >> 8) & 0xFF);

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL_0, low);
    outb(PIT_CHANNEL_0, high);
}


uint32_t timer_get_tick() {
    return tick;
}


uint32_t timer_calc_ms(uint32_t tick) {
    return (uint32_t)(tick / (freq_period / 1000));
}


uint32_t timer_calc_sec(uint32_t tick) {
    return (uint32_t)(tick / freq_period);
}


uint32_t timer_calc_tick(uint32_t ms) {
    // Not precise, approximation of current tick
    return (uint32_t)(ms * freq_period / 1000);
}


void cpu_sleep(uint32_t ms) {
    uint32_t tick_sleep_end = timer_calc_tick(
        timer_calc_ms(timer_get_tick()) + ms
    );
    do {
        asm volatile("hlt");
    } 
    while(timer_get_tick() < tick_sleep_end);
}