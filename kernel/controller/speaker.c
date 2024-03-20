#include "../include/speaker.h"
#include "../include/timer.h"
#include "../include/ports.h"

#define PIT_SPEAKER_PORT 0x61
// Other PIT macros in timer.h


void speaker_enable() {
    uint8_t temp = inb(PIT_SPEAKER_PORT);
    outb(PIT_SPEAKER_PORT, temp | 3);
}


void speaker_disable() {
    uint8_t temp = inb(PIT_SPEAKER_PORT) & 0xfc;
    outb(PIT_SPEAKER_PORT, temp);
}


void speaker_play(const uint32_t freq, const uint32_t duration) {
	if (!freq)
		return;

	speaker_enable();
	uint32_t divisor = PIT_FREQ / freq;

	outb(PIT_COMMAND, 0xb6);
	outb(PIT_CHANNEL_2, (uint8_t)(divisor));
	outb(PIT_CHANNEL_2, (uint8_t)(divisor >> 8));

	uint8_t temp = inb(PIT_SPEAKER_PORT);
	if (temp != (temp | 3))
		outb(0x61, temp | 3);

	cpu_sleep(duration);
	speaker_disable();
}