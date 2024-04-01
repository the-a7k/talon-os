#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#define PIT_FREQ 1193180
#define PIT_FREQ_DIVISOR 65536
// Timer will be invoked every (PIT_FREQ / PF_DIVISOR)

#define PIT_CHANNEL_0 0x40
#define PIT_CHANNEL_1 0x41
#define PIT_CHANNEL_2 0x42
#define PIT_COMMAND 0x43
// Macros also used in buzzer.h

void timer_init();
uint32_t timer_get_tick();
uint32_t timer_calc_ms(uint32_t tick);
uint32_t timer_calc_sec(uint32_t tick);
uint32_t timer_calc_tick(uint32_t ms);
void cpu_sleep(uint32_t ms);

#endif