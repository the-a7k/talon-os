#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stdint.h>

void speaker_enable();
void speaker_disable();
void speaker_play(const uint32_t note, const uint32_t duration);


#endif