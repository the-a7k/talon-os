#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <stdint.h>

void buzzer_enable();
void buzzer_disable();
void buzzer_play(const uint32_t freq, const uint32_t ms);

#endif