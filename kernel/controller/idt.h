#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>

/* Segment selectors */
#define KERNEL_CS_OFFSET 0x08
#define IDT_ENTRIES 256


/* How every interrupt gate (handler) is defined */
typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;


typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;


void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif