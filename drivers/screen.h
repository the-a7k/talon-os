#include <stddef.h>
#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define COL_SIZE 80
#define ROW_SIZE 25
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define DEFAULT_STR_COLOR 0x5F


void print_str(char* str, int col, int row);
void clear_row();
void clear_screen();

size_t str_len(char* str);


