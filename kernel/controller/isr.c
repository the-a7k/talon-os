#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/timer.h"
#include "../include/keyboard.h"
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/utility.h"

#define INTTERUPT_HANDLER_COUNT 256

isr_t interrupt_handlers[INTTERUPT_HANDLER_COUNT];

void isr_setup() {
    idt_gate_setup(0, (uint32_t)isr0);
    idt_gate_setup(1, (uint32_t)isr1);
    idt_gate_setup(2, (uint32_t)isr2);
    idt_gate_setup(3, (uint32_t)isr3);
    idt_gate_setup(4, (uint32_t)isr4);
    idt_gate_setup(5, (uint32_t)isr5);
    idt_gate_setup(6, (uint32_t)isr6);
    idt_gate_setup(7, (uint32_t)isr7);
    idt_gate_setup(8, (uint32_t)isr8);
    idt_gate_setup(9, (uint32_t)isr9);
    idt_gate_setup(10, (uint32_t)isr10);
    idt_gate_setup(11, (uint32_t)isr11);
    idt_gate_setup(12, (uint32_t)isr12);
    idt_gate_setup(13, (uint32_t)isr13);
    idt_gate_setup(14, (uint32_t)isr14);
    idt_gate_setup(15, (uint32_t)isr15);
    idt_gate_setup(16, (uint32_t)isr16);
    idt_gate_setup(17, (uint32_t)isr17);
    idt_gate_setup(18, (uint32_t)isr18);
    idt_gate_setup(19, (uint32_t)isr19);
    idt_gate_setup(20, (uint32_t)isr20);
    idt_gate_setup(21, (uint32_t)isr21);
    idt_gate_setup(22, (uint32_t)isr22);
    idt_gate_setup(23, (uint32_t)isr23);
    idt_gate_setup(24, (uint32_t)isr24);
    idt_gate_setup(25, (uint32_t)isr25);
    idt_gate_setup(26, (uint32_t)isr26);
    idt_gate_setup(27, (uint32_t)isr27);
    idt_gate_setup(28, (uint32_t)isr28);
    idt_gate_setup(29, (uint32_t)isr29);
    idt_gate_setup(30, (uint32_t)isr30);
    idt_gate_setup(31, (uint32_t)isr31);

    // Remaping the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 

    idt_gate_setup(32, (uint32_t)irq0);
    idt_gate_setup(33, (uint32_t)irq1);
    idt_gate_setup(34, (uint32_t)irq2);
    idt_gate_setup(35, (uint32_t)irq3);
    idt_gate_setup(36, (uint32_t)irq4);
    idt_gate_setup(37, (uint32_t)irq5);
    idt_gate_setup(38, (uint32_t)irq6);
    idt_gate_setup(39, (uint32_t)irq7);
    idt_gate_setup(40, (uint32_t)irq8);
    idt_gate_setup(41, (uint32_t)irq9);
    idt_gate_setup(42, (uint32_t)irq10);
    idt_gate_setup(43, (uint32_t)irq11);
    idt_gate_setup(44, (uint32_t)irq12);
    idt_gate_setup(45, (uint32_t)irq13);
    idt_gate_setup(46, (uint32_t)irq14);
    idt_gate_setup(47, (uint32_t)irq15);

    idt_setup(); // Load with ASM
}


void isr_handler(registers_t *reg) {
    char int_count[4];
    itoa(reg->int_num, int_count);
    kprint_color("The system has halted! (int ", TTY_BLACK, TTY_RED);
    kprint_color(int_count, TTY_BLACK, TTY_RED);
    kprint_color(")\n", TTY_BLACK, TTY_RED);
    asm("hlt");
}


void interrupt_handler_install(uint8_t num, isr_t handler) {
    interrupt_handlers[num] = handler;
}


void irq_handler(registers_t *reg) {
    if (reg->int_num >= 40)
        outb(0xA0, 0x20);  // Slave
    outb(0x20, 0x20);  // Master

    if (interrupt_handlers[reg->int_num] != 0) {
        isr_t handler = interrupt_handlers[reg->int_num];
        handler(reg);
    }
}


void irq_setup() {
    asm volatile("sti");   // Enabling interrupts
    timer_init();          // IRQ 01 timer
    keyboard_init();       // IRQ 02 keyboard
}