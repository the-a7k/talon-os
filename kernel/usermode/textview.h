#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void tm_background(uint8_t color);
void tm_navigation(uint8_t row, uint8_t bg, uint8_t options_fg, char* options[], size_t total_options);
void tm_rectangle(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg);
void tm_plane(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg, uint8_t line_color, bool single_line);
void tm_window(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg, uint8_t line_color, bool single_line);
void tm_shadow(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row);
void tm_label(char *str, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg);

#endif