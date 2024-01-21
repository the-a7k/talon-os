#include "screen.h"
#include "ports.h"


/* Private method declaration */

int print_cell(char c, uint8_t color, int col, int row);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);


/* Public methods */


void print_str(char* str, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }

    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    for (size_t i = 0; i < str_len(str); i++) {
        offset = print_cell(str[i], DEFAULT_STR_COLOR, col, row);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}


void clear_row(int row) {
    for (size_t x = 0; x < COL_SIZE; x++) {
        print_cell(0, NULL, x, row);
    }
}


void clear_screen() {
    for (size_t y = 0; y < ROW_SIZE; y++) {
        clear_row(y);
    }
    set_cursor_offset(get_offset(0,0));
}


/* Private methods */


int print_cell(char c, uint8_t color, int col, int row) {
    volatile unsigned char* vga_buffer = (unsigned char*) VIDEO_MEMORY;

    if (col >= COL_SIZE || row >= ROW_SIZE) {
        return get_offset(col,row);
    }

    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor_offset();
    }

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    }
    else {
        vga_buffer[offset] = c;
        //vga_buffer[offset+1] = color;
        offset += 2;
    }

    set_cursor_offset(offset);
    return offset;
}


int get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}


void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}


int get_offset(int col, int row) { 
    return 2 * (row * COL_SIZE + col); 
}


int get_offset_row(int offset) { 
    return offset / (2 * COL_SIZE); 
}


int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset) * 2 * COL_SIZE)) / 2; 
}


// TODO: Move this to custom string lib
size_t str_len(char* str) {
    size_t size = 0;
    while (str[size] != '\0') {
        size++;
    }
    return size;
}