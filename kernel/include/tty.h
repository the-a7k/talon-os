#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stdbool.h>

#define COL_SIZE 80
#define ROW_SIZE 25

enum TEXMODE_COLORS {
    TTY_BLACK,
    TTY_BLUE,
    TTY_GREEN,
    TTY_CYAN,
    TTY_RED,
    TTY_MAGENTA,
    TTY_BROWN,
    TTY_LIGHT_GREY,
    TTY_DARK_GREY,
    TTY_LIGHT_BLUE,
    TTY_LIGHT_GREEN,
    TTY_LIGHT_CYAN,
    TTY_PINK,
    TTY_LIGHT_MAGENTA,
    TTY_YELLOW,
    TTY_WHITE
};

// TTY basic output
void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg);
void kprint_color(char *str, uint8_t bg, uint8_t fg);
void kprint(char *str);
void kputchar(char c);
void kprintint(int num);
void kprinthex(int num);
void error_msg(char *reason);

// Screen position calculations
uint16_t calc_pos(uint8_t col, uint8_t row);
uint8_t calc_row(uint16_t pos);
uint8_t calc_col(uint16_t pos);
bool pos_valid(uint8_t col, uint8_t row);

// Screen color manipulation and screen clearing
void set_cell_color(uint8_t col, uint8_t row, uint8_t bg);
void set_row_color(uint8_t row, uint8_t bg);
void set_screen_color(uint8_t bg);
void clear_cell(uint8_t col, uint8_t row);
void clear_cell_cursor();
void clear_row(uint8_t row);
void clear_screen();

// TTY helper functions
void newline();
void tab();
void scroll();

// Cursor manipulation
void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void cursor_disable();
void cursor_advance();
void cursor_retreat();
bool cursor_valid();
void cursor_move(uint8_t col, uint8_t row);
uint16_t get_cursor_pos();

// Basic CLI setup
void tty_setup();

#endif

