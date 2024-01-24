#include "tty.h"
#include "ports.h"
#include "../kernel/util.h"


void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg) {
    volatile uint8_t* vga_buffer = (uint8_t*) VIDEO_MEMORY;
    uint16_t pos = calc_pos(col, row) / 2;
    vga_buffer[pos * 2] = c;
    vga_buffer[pos * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}


void kprint_color(char* str, uint8_t bg, uint8_t fg) {
    size_t i = 0;
    while (str[i] != '\0') {
        uint16_t pos = get_cursor_pos(); 
        if (pos >= ROW_SIZE * COL_SIZE * 2) {
            scroll();
            pos -= 2 * COL_SIZE;
        }

        if (str[i] == '\n' || str[i] == '\r') {
            newline();
        }

        else if (str[i] == '\t') {
            tab();
        }

        else {
            uint8_t col = calc_col(pos);
            uint8_t row = calc_row(pos);
            write_cell(str[i], col, row, bg, fg);
            cursor_advance();
            pos += 2;
        }

        i++;
    }
}


void kprint(char* str) {
    kprint_color(
        str, 
        BLACK, 
        LIGHT_GREY
    );
}


void clear_row(uint8_t row) {
    for (size_t x = 0; x < COL_SIZE; x++) {
        write_cell(0, x, row, BLACK, LIGHT_GREY);
    }
    move_cursor(0, row);
} 


void clear_screen() {
    for (size_t y = 0; y < ROW_SIZE; y++) {
        clear_row(y);
    }
    move_cursor(0, 0);
}


void newline() {
    uint8_t row = calc_row(get_cursor_pos()) + 1;
    move_cursor(0, row);
}


void tab() {
    for (size_t i = 0; i < 4; i++) {
        cursor_advance();
    }
}


void scroll() {
    for (size_t i = 1; i < ROW_SIZE; i++) {
        memory_copy(
            calc_pos(0, i) + VIDEO_MEMORY,
            calc_pos(0, i - 1) + VIDEO_MEMORY,
            COL_SIZE * 2
        );
    }
    clear_row(ROW_SIZE - 1);
    move_cursor(0, ROW_SIZE - 1);
}


void cursor_advance() {
    uint16_t pos = get_cursor_pos() + 2;
    move_cursor(
        calc_col(pos), 
        calc_row(pos)
    );
}


void cursor_retreat() {
    uint16_t pos = get_cursor_pos() - 2;
    move_cursor(
        calc_col(pos), 
        calc_row(pos)
    );
}


void move_cursor(uint8_t col, uint8_t row) {
    uint16_t pos = calc_pos(col, row) / 2;
    outb(VGA_CTRL_PORT, VGA_HIGH_BYTE);
    outb(VGA_DATA_PORT, (unsigned char)(pos >> 8));
    outb(VGA_CTRL_PORT, VGA_LOW_BYTE);
    outb(VGA_DATA_PORT, (unsigned char)(pos & 0xff));
}


uint16_t get_cursor_pos() {
    outb(VGA_CTRL_PORT, 14);
    uint16_t pos = inb(VGA_DATA_PORT) << 8;
    outb(VGA_CTRL_PORT, 15);
    pos += inb(VGA_DATA_PORT);
    return pos * 2;
}


uint16_t calc_pos(uint8_t col, uint8_t row) { 
    return 2 * (row * COL_SIZE + col); 
}


uint8_t calc_col(uint16_t pos) { 
    return (pos - (calc_row(pos) * 2 * COL_SIZE)) / 2; 
}


uint8_t calc_row(uint16_t pos) { 
    return pos / (2 * COL_SIZE); 
}