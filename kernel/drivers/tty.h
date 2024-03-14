#ifndef _TTY_H_
#define _TTY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define COL_SIZE 80
#define ROW_SIZE 25

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


typedef struct {
    uint8_t src_col;
    uint8_t src_row;
    uint8_t dest_col;
    uint8_t dest_row;
    uint16_t cursor_pos;
    bool is_scrollable;
    bool is_active;
} TextRegion;


// TTY basic output
void write_cell(char c, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg);
void kprint_color(char *str, uint8_t bg, uint8_t fg);
void kprint(char *str);
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
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void cursor_advance();
void cursor_retreat();
bool cursor_valid();
void move_cursor(uint8_t col, uint8_t row);
uint16_t get_cursor_pos();

// Specify text-writable region
void tty_setup();
void create_text_region(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, bool is_scrollable);
void text_region_activate(size_t num);

// Text region getters (shortened to 'tr')
TextRegion* tr_get(size_t num);
size_t tr_get_size();
size_t tr_get_active();
uint8_t tr_get_src_col(size_t num);
uint8_t tr_get_src_row(size_t num);
uint8_t tr_get_dest_col(size_t num);
uint8_t tr_get_dest_row(size_t num);
uint16_t tr_get_cursor(size_t num);

#endif

