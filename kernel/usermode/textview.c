#include "textview.h"
#include "../drivers/tty.h"
#include "../libc/string.h"

#define TAB_SIZE 3

const uint8_t ascii_double_line[] = {
    201,  // BORDER_VERT_TOPRIGHT
    187,  // BORDER_VERT_TOPLEFT
    200,  // BORDER_VERT_BOTRIGHT
    188,  // BORDER_VERT_BOTLEFT
    205,  // BORDER_HORIZONTAL
    186   // BORDER_VERTICAL
};

const uint8_t ascii_single_line[] = {
    218,  // LINE_VERT_TOPRIGHT
    191,  // LINE_VERT_TOPLEFT
    192,  // LINE_VERT_BOTRIGHT
    217,  // LINE_VERT_BOTLEFT
    196,  // LINE_HORIZONTAL
    179   // LINE_VERTICAL
};


void tm_set_background(uint8_t color) {
    set_screen_color(color);
}


void tm_rectangle(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg) {
    for (size_t y = src_row; y <= dest_row; y++) {
        for (size_t x = src_col; x <= dest_col; x++) {
            set_cell_color(x, y, bg);  
        }
    }
}


void tm_create_shadow(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row) {
    tm_rectangle(src_col+1, src_row+1, dest_col+2, dest_row+1, BLACK);
}


void tm_navigation(uint8_t row, uint8_t bg, uint8_t options_fg, char* options[], size_t total_options) {
    set_row_color(row, bg);
    uint8_t current_col = 1;
    for (size_t i = 0; i < total_options; i++) {
        tm_set_text(options[i], current_col, row, bg, options_fg);
        current_col += strlen(options[i]) + TAB_SIZE;
    }
}


void tm_plane(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg, uint8_t line_color, bool single_line) {
    const uint8_t *border_styles = single_line ? ascii_single_line : ascii_double_line;  
    for (size_t y = src_row; y <= dest_row; y++) {
        for (size_t x = src_col; x <= dest_col; x++) {
            set_cell_color(x, y, bg);

            if (y == src_row && x == src_col) {
                write_cell(border_styles[0], x, y, bg, line_color);
            } 
            else if (y == src_row && x == dest_col) {
                write_cell(border_styles[1], x, y, bg, line_color);
            } 
            else if (y == dest_row && x == src_col) {
                write_cell(border_styles[2], x, y, bg, line_color);
            } 
            else if (y == dest_row && x == dest_col) {
                write_cell(border_styles[3], x, y, bg, line_color);
            } 
            else if (y == src_row || y == dest_row) {
                write_cell(border_styles[4], x, y, bg, line_color);
            } 
            else if (y == src_row || x == src_col || y == dest_row || x == dest_col) {
                write_cell(border_styles[5], x, y, bg, line_color);
            }
        }
    }
}


void tm_window(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg, uint8_t line_color, bool single_line) {
    tm_create_shadow(src_col, src_row, dest_col, dest_row);
    tm_plane(src_col, src_row, dest_col, dest_row, bg, line_color, single_line);
}


void tm_set_text(char *str, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg) {
    size_t i = 0;
    while (str[i] != '\0') {
        if (calc_pos(col,row) >= (ROW_SIZE * COL_SIZE * 2) - 1) {
            break;
            return;
        }
        if ((str[i] == '\n' || str[i] == '\r') && pos_valid(0, row+1)) {
            col = 0;
            row++;
        }
        else if (str[i] == '\t') {
            col += TAB_SIZE;
        }
        else {
            write_cell(str[i], col, row, bg, fg);
            col++;
        }
        i++;
    }
}