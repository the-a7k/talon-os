#include "timer.h"
#include "isr.h"
#include "../drivers/ports.h"

static volatile uint32_t tick = 0;

uint32_t get_tick() {
    return tick;
}


uint32_t calc_milisecond(uint32_t tick) {
    // Is this even working?
    float freq_divisor = PIT_FREQ / PIT_FREQ_DEFAULT;
    float tick_time = tick / (60 * (1 / freq_divisor)) / 10;
    return (uint32_t)tick_time;
}


uint32_t calc_second(uint32_t tick) {
    return calc_milisecond(tick) / 1000;
}


uint32_t calc_tick(uint32_t ms) {
    float freq_divisor = PIT_FREQ / PIT_FREQ_DEFAULT;
    float tick_time = ms * (60 * (1 / freq_divisor)) * 10;
    return (uint32_t)tick_time;
}


void cpu_sleep(uint32_t ms) {
    volatile uint32_t tick_sleep_end = calc_tick(
        calc_milisecond(get_tick()) + ms);
    do {
        asm("hlt");
    }
    while(get_tick() < tick_sleep_end);
}


static void timer_callback(registers_t *reg) {
    tick++;
}


void init_timer() {
    interrupt_handler_install(IRQ0, timer_callback);

    uint32_t divisor = PIT_FREQ / PIT_FREQ_DEFAULT;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_COMMAND, 0x36);  // Command port
    outb(PIT_CHANNEL_0, low);
    outb(PIT_CHANNEL_0, high);
}