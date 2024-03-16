#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

void memcpy(void *dest, const void  *source, uint32_t count);
void memset(void *dest, const uint32_t value, uint32_t count);

#endif