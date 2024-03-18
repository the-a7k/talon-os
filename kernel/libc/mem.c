#include <stddef.h>
#include "mem.h"


void memcpy(void *dest, const void *source, uint32_t count) {
    uint16_t *dest_adr = (uint16_t *)dest;
    const uint16_t *src_adr = (const uint16_t *)source;
    while (count--)
        *dest_adr++ = *src_adr++;
}


void memset(void *dest, const uint32_t value, uint32_t count) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; count != 0; --count) 
        *temp++ = value;
}