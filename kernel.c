#include <stdint.h>
#include <stddef.h>
const int X_SIZE = 80;
const int Y_SIZE = 25;


void print_char(unsigned char c, uint8_t color, int x, int y) {
    volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
    uint16_t attrib = (color << 4) | 0x0F;
    vga_buffer[y * 80 + x] = (c | (attrib << 8));
}


void clear_screen() {
    volatile uint16_t* vga_buffer = (uint16_t*)0xB8000; 
    for (size_t y = 0; y < Y_SIZE; y++) {
        for (size_t x = 0; x < X_SIZE; x++) {
            vga_buffer[y * 80 + x] = 0;
        }
    }
}


void main() {
    clear_screen();
    print_char('H', 0xF0, 0, 0);
    print_char('e', 0xF0, 1, 0);
    print_char('l', 0xF0, 2, 0);
    print_char('l', 0xF0, 3, 0);
    print_char('o', 0xF0, 4, 0);
    print_char('!', 0xF0, 5, 0);
    print_char('X', 0xF0, 79, 24);
}