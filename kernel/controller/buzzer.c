#include "../include/buzzer.h"
#include "../include/timer.h"
#include "../include/ports.h"

#define PIT_BUZZER_PORT 0x61
// Other PIT macros in timer.h


void buzzer_enable() {
    uint8_t temp = inb(PIT_BUZZER_PORT);
    outb(PIT_BUZZER_PORT, temp | 3);
}


void buzzer_disable() {
    uint8_t temp = inb(PIT_BUZZER_PORT) & 0xfc;
    outb(PIT_BUZZER_PORT, temp);
}


void buzzer_play(const uint32_t freq, const uint32_t ms) {
    if (!freq)
        return;

    buzzer_enable();
    uint32_t divisor = PIT_FREQ / freq;

    outb(PIT_COMMAND, 0xb6);
    outb(PIT_CHANNEL_2, (uint8_t)(divisor));
    outb(PIT_CHANNEL_2, (uint8_t)(divisor >> 8));

    uint8_t temp = inb(PIT_BUZZER_PORT);
    if (temp != (temp | 3))
        outb(0x61, temp | 3);

    cpu_sleep(ms + 15);  // Buzzer end offset
    buzzer_disable();
}