#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stdbool.h>

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
    PINK,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};


// TTY output
void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg);
void kprint_color(char *str, uint8_t bg, uint8_t fg);
void kprint(char *str);
void error_msg(char *reason);

// Screen clearing
void clear_cell(uint8_t col, uint8_t row);
void clear_row(uint8_t row);
void clear_screen();

// TTY helper functions
void newline();
void tab();
void scroll();

// Screen position calculations
uint16_t calc_memory_pos(uint8_t col, uint8_t row);
uint16_t calc_pos(uint8_t col, uint8_t row);
uint8_t calc_row(uint16_t pos);
uint8_t calc_col(uint16_t pos);
bool pos_valid(uint8_t col, uint8_t row);

// Cursor manipulation
void cursor_advance();
void cursor_retreat();
bool cursor_valid();
void move_cursor(uint8_t col, uint8_t row);
uint16_t get_cursor_pos();

#endif

