#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

#define PIT_FREQ 1193180
#define PIT_FREQ_DEFAULT 65536
#define PIT_CHANNEL_0 0x40
#define PIT_CHANNEL_1 0x41
#define PIT_CHANNEL_2 0x42
#define PIT_COMMAND 0x43


void init_timer();
uint32_t get_tick();
uint32_t calc_milisecond(uint32_t tick);
uint32_t calc_second(uint32_t tick);
uint32_t calc_tick(uint32_t ms);
void cpu_sleep(uint32_t ms);

#endif