#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include "../../libc/util.h"

#define PIT_FREQ 1193180

void init_timer(uint32_t freq);

#endif