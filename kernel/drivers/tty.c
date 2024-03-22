#include <stddef.h>
#include "../include/tty.h"
#include "../include/ports.h"
#include "../include/mem.h"
#include "../include/utility.h"
#include "../include/command.h"

#define VIDEO_MEMORY 0xb8000
#define VGA_ADDRESS_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5
#define VGA_HIGH 14
#define VGA_LOW 15


void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg) {
    volatile uint8_t *vga_buffer = (uint8_t*) VIDEO_MEMORY;
    uint16_t pos = calc_pos(col, row);
    vga_buffer[pos] = c;
    vga_buffer[pos + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}


void kprint_color(char *str, uint8_t bg, uint8_t fg) {
    size_t i = 0;
    while (str[i] != '\0') {
        uint16_t pos = get_cursor_pos(); 
        bool cursor_overflow = false;

        if (pos >= (ROW_SIZE * COL_SIZE * 2)) {
            scroll();
            pos = get_cursor_pos();
            bool cursor_overflow = true;
        }

        if (str[i] == '\n') {
            if (!cursor_overflow)
                newline();
        }
        else if (str[i] == '\t')
            tab();
        else {
            write_cell(str[i], calc_col(pos), calc_row(pos), bg, fg);
            cursor_advance();
            pos += 2;
        }
        i++;
    }
}


void kprint(char *str) {
    kprint_color(str, TTY_BLACK, TTY_LIGHT_GREY);
}


void kputchar(char c) {
    char char_build[sizeof(c) * 2] = {c, '\0'};
    kprint(char_build);
}


void kprintint(int num) {
    char int_build[32];
    itoa(num, int_build);
    kprint(int_build);
}


void error_msg(char *reason) {
    kprint_color("\nAn error has occured! Reason: ", TTY_BLACK, TTY_RED);
    kprint_color(reason, TTY_BLACK, TTY_PINK);
    newline();
}


bool pos_valid(uint8_t col, uint8_t row) {
    return (col < COL_SIZE) && (row < ROW_SIZE);
}


uint16_t calc_pos(uint8_t col, uint8_t row) {
    return 2 * (row * COL_SIZE + col);  // For cursors: calc_pos / 2
}


uint8_t calc_col(uint16_t pos) { 
    return (pos - (calc_row(pos) * 2 * COL_SIZE)) / 2; 
}


uint8_t calc_row(uint16_t pos) { 
    return pos / (2 * COL_SIZE); 
}


void set_cell_color(uint8_t col, uint8_t row, uint8_t bg) {
    if (!pos_valid(col, row)) {
        error_msg("Screen cell location out of bounds");
        return;
    }
    write_cell(' ', col, row, bg, TTY_LIGHT_GREY);
}


void set_row_color(uint8_t row, uint8_t bg) {
    if (!pos_valid(0, row)) {
        error_msg("Screen row location out of bounds");
        return;
    }
    for (size_t x = 0; x < COL_SIZE; x++)
        set_cell_color(x, row, bg);
}


void set_screen_color(uint8_t bg) {
    for (size_t y = 0; y < ROW_SIZE; y++)
        set_row_color(y, bg);
}


void clear_cell(uint8_t col, uint8_t row) {
    set_cell_color(col, row, TTY_BLACK);
}


void clear_cell_cursor() {
    set_cell_color(
        calc_col(get_cursor_pos()),
        calc_row(get_cursor_pos()),
        TTY_BLACK
    );
}


void clear_row(uint8_t row) {
    set_row_color(row, TTY_BLACK);
}


void clear_screen() {
    set_screen_color(TTY_BLACK);
    move_cursor(0,0);
}


void newline() {
    uint8_t current_row = calc_row(get_cursor_pos());
    if (current_row >= ROW_SIZE - 1)
        scroll();
    else
        move_cursor(0, current_row + 1);
}


void tab() {
    for (size_t i = 0; i < 8; i++)
        cursor_advance();
}


void scroll() {
    for (size_t i = 1; i < ROW_SIZE; i++) {
        memcpy(
            (uint16_t *)(calc_pos(0, i - 1) + VIDEO_MEMORY),
            (uint16_t *)(calc_pos(0, i) + VIDEO_MEMORY),
            (uint32_t)COL_SIZE * 2
        );
    }
    clear_row(ROW_SIZE - 1);
    move_cursor(0, ROW_SIZE - 1);
}


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(VGA_ADDRESS_PORT, 0x0A);
    outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xC0) | cursor_start);
    outb(VGA_ADDRESS_PORT, 0x0B);
    outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xE0) | cursor_end);
}


void disable_cursor() {
    outb(VGA_ADDRESS_PORT, 0x0A);
    outb(VGA_DATA_PORT, 0x20);
}


void cursor_advance() {
    uint16_t pos = get_cursor_pos() + 2;
    if ((pos / 2) >= (COL_SIZE * ROW_SIZE))
        scroll();
    else {
        move_cursor(
            calc_col(pos), 
            calc_row(pos)
        );
    }
}


void cursor_retreat() {
    uint16_t pos = get_cursor_pos();
    if (pos != 0) {
        move_cursor(
            calc_col(pos-2), 
            calc_row(pos-2)
        );
    }
}


bool cursor_valid() {
    return pos_valid(
        calc_col(get_cursor_pos()),
        calc_row(get_cursor_pos())
    );
}


void move_cursor(uint8_t col, uint8_t row) {
    if (!cursor_valid()) {
        clear_screen();
        error_msg("Cursor out of bounds");
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
        outb(VGA_ADDRESS_PORT, VGA_HIGH);
        outb(VGA_DATA_PORT, (uint8_t)(pos >> 8));
        outb(VGA_ADDRESS_PORT, VGA_LOW);
        outb(VGA_DATA_PORT, (uint8_t)(pos & 0xff));
    }
}


uint16_t get_cursor_pos() {
    outb(VGA_ADDRESS_PORT, VGA_HIGH);
    uint16_t pos = inb(VGA_DATA_PORT) << 8;
    outb(VGA_ADDRESS_PORT, VGA_LOW);
    pos += inb(VGA_DATA_PORT);
    return pos * 2;
}


void tty_setup() {
    clear_screen();
    kprint_color("Welcome to talonOS! Type 'help' to get started.\n", TTY_BLACK, TTY_LIGHT_BLUE);
    kprint((char *)CLI_PREFIX);
}