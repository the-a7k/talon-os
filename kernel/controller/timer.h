#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

void init_timer(uint32_t freq);
uint32_t get_tick();

#endif