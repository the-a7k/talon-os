#include <stdbool.h>
#include "tty.h"
#include "ports.h"
#include "../../libc/util.h"



void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg) {
    volatile uint8_t *vga_buffer = (uint8_t*) VIDEO_MEMORY;
    uint16_t pos = calc_pos(col, row) / 2;
    vga_buffer[pos * 2] = c;
    vga_buffer[pos * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}


void kprint_color(char *str, uint8_t bg, uint8_t fg) {
    size_t i = 0;
    while (str[i] != '\0') {
        uint16_t pos = get_cursor_pos(); 
        bool cursor_overflow = false;

        if (pos >= ROW_SIZE * COL_SIZE * 2) {
            scroll();
            pos -= 2 * COL_SIZE;
            bool cursor_overflow = true;
        }

        if (str[i] == '\n' || str[i] == '\r') {
            if (!cursor_overflow) {
                newline();
            }
                
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


void kprint(char *str) {
    kprint_color(
        str, 
        BLACK, 
        LIGHT_GREY
    );
}


void error_msg(char *reason) {
    kprint_color("\nAn error has occured! Reason: ", BLACK, RED);
    kprint_color(reason, BLACK, PINK);
    newline();
}


void clear_row(uint8_t row) {
    for (size_t x = 0; x < COL_SIZE; x++) {
        write_cell(0, x, row, BLACK, LIGHT_GREY);
    }
} 


void clear_screen() {
    for (size_t y = 0; y < ROW_SIZE; y++) {
        clear_row(y);
    }
    move_cursor(0, 0);
}


void newline() {
    uint8_t current_row = calc_row(get_cursor_pos());
    if (current_row >= ROW_SIZE - 1) {
        scroll();
    }
    else {
        move_cursor(0, current_row+1);
    }
    
}


void tab() {
    for (size_t i = 0; i < 8; i++) {
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


bool pos_valid(uint8_t col, uint8_t row) {
    return (col < COL_SIZE) && (row < ROW_SIZE);
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


bool cursor_valid() {
    return pos_valid(
        calc_col(get_cursor_pos()),
        calc_row(get_cursor_pos())
    );
}


void move_cursor(uint8_t col, uint8_t row) {
    if (!cursor_valid()) {
        move_cursor(0,0);
        error_msg("Cursor out of bounds (position set to 0)");
    }

    else if (cursor_valid() && !pos_valid(col, row)) {
        error_msg("Illegal cursor position");
        move_cursor(
            calc_col(get_cursor_pos()),
            calc_row(get_cursor_pos())
        );
    }

    else {
        uint16_t pos = calc_pos(col, row) / 2;
        outb(VGA_ADDRESS_PORT, VGA_HIGH_BYTE);
        outb(VGA_DATA_PORT, (uint8_t)(pos >> 8));
        outb(VGA_ADDRESS_PORT, VGA_LOW_BYTE);
        outb(VGA_DATA_PORT, (uint8_t)(pos & 0xff));
    }
}


uint16_t get_cursor_pos() {
    outb(VGA_ADDRESS_PORT, 14);
    uint16_t pos = inb(VGA_DATA_PORT) << 8;
    outb(VGA_ADDRESS_PORT, 15);
    pos += inb(VGA_DATA_PORT);
    return pos * 2;
}