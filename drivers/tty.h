#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY 0xb8000
#define COL_SIZE 80
#define ROW_SIZE 25

#define VGA_CTRL_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5
#define VGA_HIGH_BYTE 14
#define VGA_LOW_BYTE 15


enum COLORS_16 {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};


// TTY output
void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg);
void kprint_color(char* str, uint8_t bg, uint8_t fg);
void kprint(char* str);

// Screen clearing
void clear_row(uint8_t row);
void clear_screen();

// TTY helper functions
void newline();
void tab();
void scroll();

// Cursor manipulation
void cursor_advance();
void cursor_retreat();
void move_cursor(uint8_t col, uint8_t row);
uint16_t get_cursor_pos();

// Screen position calculations
uint16_t calc_pos(uint8_t col, uint8_t row);
uint8_t calc_row(uint16_t pos);
uint8_t calc_col(uint16_t pos);

#endif

