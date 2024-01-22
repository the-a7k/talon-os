#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"


/* Private method declaration */


int print_cell(char c, int color, int col, int row);
int get_cursor_offset();
void set_cursor_offset(int offset);
int calc_offset(int col, int row);
int calc_row(int offset);
int calc_col(int offset);


/* Public methods */


void print_at(char* str, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0) {
        offset = calc_offset(col, row);
    }

    else {
        offset = get_cursor_offset();
        row = calc_row(offset);
        col = calc_col(offset);
    }

    int i = 0;
    while (str[i] != '\0') {
        offset = print_cell(str[i++], DEFAULT_STR_COLOR, col, row);
        row = calc_row(offset);
        col = calc_col(offset);
    }
}


void print_cursor(char* str) {
    print_at(
        str, 
        calc_col(get_cursor_offset()), 
        calc_row(get_cursor_offset())
    );
}


void clear_row(int row) {
    int cursor_current = get_cursor_offset();
    for (int x = 0; x < COL_SIZE; x++) {
        print_cell(0, 0, x, row);
    }

    set_cursor_offset(
        calc_offset(
            calc_col(cursor_current), 
            calc_row(cursor_current)
        )
    );
}   


void clear_screen() {
    for (int y = 0; y < ROW_SIZE; y++) {
        clear_row(y);
    }
    set_cursor_offset(calc_offset(0,0));
}


/* Private methods */


int print_cell(char c, int color, int col, int row) {
    volatile unsigned char* vga_buffer = (unsigned char*) VIDEO_MEMORY;

    if (col >= COL_SIZE || row >= ROW_SIZE) {
        return calc_offset(col,row);
    }

    int offset;
    if (col >= 0 && row >= 0) {
        offset = calc_offset(col, row);
    }
    else {
        offset = get_cursor_offset();
    }

    if (c == '\n') {
        row = calc_row(offset);
        offset = calc_offset(0, row + 1);
    }
    else {
        vga_buffer[offset] = c;
        vga_buffer[offset+1] = color;
        offset += 2;
    }


    if (offset >= ROW_SIZE * COL_SIZE * 2) {
        for (int i = 1; i < ROW_SIZE; i++) {
            memory_copy(
                calc_offset(0, i) + VIDEO_MEMORY,
                calc_offset(0, i-1) + VIDEO_MEMORY,
                COL_SIZE * 2
            );
        }

        char* last_line = calc_offset(0, ROW_SIZE-1) + VIDEO_MEMORY;
        for (int i = 0; i < COL_SIZE * 2; i++) {
            last_line[i] = 0;
        }
        offset -= 2 * COL_SIZE;
    }

    set_cursor_offset(offset);
    return offset;
}


int get_cursor_offset() {
    outb(REG_SCREEN_CTRL, 14);
    int offset = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 15);
    offset += inb(REG_SCREEN_DATA);
    return offset * 2;
}


void set_cursor_offset(int offset) {
    offset /= 2;
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}


int calc_offset(int col, int row) { 
    return 2 * (row * COL_SIZE + col); 
}


int calc_row(int offset) { 
    return offset / (2 * COL_SIZE); 
}


int calc_col(int offset) { 
    return (offset - (calc_row(offset) * 2 * COL_SIZE)) / 2; 
}