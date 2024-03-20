#include "../include/idt.h"

#define KERNEL_CS_OFFSET 0x08
#define IDT_ENTRIES 255

idt_entry_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void idt_gate_setup(uint8_t num, uint32_t handler) {
    idt[num].isr_low = (uint32_t)handler & 0xFFFF;
    idt[num].kernel_cs = KERNEL_CS_OFFSET;
    idt[num].reserved = 0;
    idt[num].attributes = 0x8E; 
    idt[num].isr_high = (uint32_t)handler >> 16;
}

void idt_setup() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1;
    asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}