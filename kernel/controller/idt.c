#include "idt.h"

idt_entry_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler) {
    idt[n].isr_low = (uint32_t)handler & 0xFFFF;
    idt[n].kernel_cs = KERNEL_CS_OFFSET;
    idt[n].reserved = 0;
    idt[n].attributes = 0x8E; 
    idt[n].isr_high = (uint32_t)handler >> 16;
}

void set_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_entry_t) - 1;
    asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}