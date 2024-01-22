#include "ports.h"

//  Read a byte from the specified port

unsigned char inb (unsigned short port) {
    unsigned char result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}


void outb (unsigned short port, unsigned char data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}


unsigned short inw (unsigned short port) {
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}


void outw (unsigned short port, unsigned short data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}