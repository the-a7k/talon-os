#include "tty.h"
#include "ports.h"
#include "../libc/util.h"


#define VIDEO_MEMORY 0xb8000
#define VGA_ADDRESS_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5
#define VGA_HIGH_BYTE 14
#define VGA_LOW_BYTE 15


static TextRegion text_region[16];
static size_t text_region_size = 0;
static size_t text_region_active = 0;


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

        /*
        if (pos >= (ROW_SIZE * COL_SIZE * 2) - 1) {
            scroll();
            pos = get_cursor_pos();
            bool cursor_overflow = true;
        }
        */

        if (str[i] == '\n' || str[i] == '\r') {
            if (!cursor_overflow) {
                newline();
                write_cell('L',20,0,50,0);
            }
        }
        else if (str[i] == '\t') {
            tab();
            write_cell('L',10,0,50,0);
        }
        else {
            write_cell(str[i], calc_col(pos), calc_row(pos), bg, fg);
            cursor_advance();
            pos += 2;
        }
        i++;
    }
}


void kprint(char *str) {
    kprint_color(str, BLACK, LIGHT_GREY);
}


void error_msg(char *reason) {
    kprint_color("\nAn error has occured! Reason: ", BLACK, RED);
    kprint_color(reason, BLACK, PINK);
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
    write_cell(' ', col, row, bg, LIGHT_GREY);
}


void set_row_color(uint8_t row, uint8_t bg) {
    if (!pos_valid(0, row)) {
        error_msg("Screen row location out of bounds");
        return;
    }
    for (size_t x = 0; x < COL_SIZE; x++) {
        set_cell_color(x, row, bg);
    }
}


void set_screen_color(uint8_t bg) {
    for (size_t y = 0; y < ROW_SIZE; y++) {
        set_row_color(y, bg);
    }
}


void clear_cell(uint8_t col, uint8_t row) {
    set_cell_color(col, row, BLACK);
}


void clear_row(uint8_t row) {
    set_row_color(row, BLACK);
}


void clear_screen() {
    set_screen_color(BLACK);
}


void newline() {
    TextRegion *tr = tr_get(tr_get_active());
    /*
    if (calc_row(tr->cursor_pos) >= ROW_SIZE - 1) {
        //scroll();
        return;
    }
    */
    if (calc_row(tr->cursor_pos) < tr->dest_row) {
        move_cursor(
            tr->src_col, 
            calc_row(tr->cursor_pos) + 1);
    }
}


void tab() {
    for (size_t i = 0; i < 8; i++) {
        cursor_advance();
    }
}


void scroll() {
    for (size_t i = 1; i < ROW_SIZE; i++) {
        memory_copy(
            calc_pos(0, i) + VIDEO_MEMORY,
            calc_pos(0, i - 1) + VIDEO_MEMORY,
            COL_SIZE * 2
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
    TextRegion *tr = tr_get(tr_get_active());

    if (calc_col(tr->cursor_pos) >= tr->dest_col && calc_row(tr->cursor_pos) >= tr->dest_row) {
        return;
    }
    else if (calc_col(tr->cursor_pos) >= tr->dest_col) {
        move_cursor(
            calc_col(tr->src_col*2), 
            calc_row(tr->cursor_pos)+1
        );
    }
    else {
        move_cursor(
            calc_col(tr->cursor_pos+2), 
            calc_row(tr->cursor_pos+2)
        );
    }
}


void cursor_retreat() {
    TextRegion *tr = tr_get(tr_get_active());

    if (calc_col(tr->cursor_pos) <= tr->src_col && calc_row(tr->cursor_pos) <= tr->src_row) {
        return;
    }
    if (calc_col(tr->cursor_pos) <= tr->src_col) {
        move_cursor(
            calc_col(tr->dest_col*2), 
            calc_row(tr->cursor_pos)-1
        );
    }
    else {
        move_cursor(
            calc_col(tr->cursor_pos-2), 
            calc_row(tr->cursor_pos-2)
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
        outb(VGA_ADDRESS_PORT, VGA_HIGH_BYTE);
        outb(VGA_DATA_PORT, (uint8_t)(pos >> 8));
        outb(VGA_ADDRESS_PORT, VGA_LOW_BYTE);
        outb(VGA_DATA_PORT, (uint8_t)(pos & 0xff));
        text_region[tr_get_active()].cursor_pos = pos * 2;
    }
}


uint16_t get_cursor_pos() {
    outb(VGA_ADDRESS_PORT, VGA_HIGH_BYTE);
    uint16_t pos = inb(VGA_DATA_PORT) << 8;
    outb(VGA_ADDRESS_PORT, VGA_LOW_BYTE);
    pos += inb(VGA_DATA_PORT);
    return pos * 2;
}


void tty_setup() {
    create_text_region(0, 0, COL_SIZE - 1, ROW_SIZE - 1, false);
    text_region_activate(0);
    clear_screen();
}


void create_text_region(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, bool is_scrollable) {
    text_region[text_region_size].src_col = src_col;
    text_region[text_region_size].src_row = src_row;
    text_region[text_region_size].dest_col = dest_col;
    text_region[text_region_size].dest_row = dest_row;
    text_region[text_region_size].cursor_pos = calc_pos(src_col, src_row);
    text_region[text_region_size].is_scrollable = is_scrollable;
    text_region[text_region_size].is_active = false;
    text_region_size++;
}


void text_region_activate(size_t num) {
    text_region[tr_get_active()].is_active = false;
    text_region[num].is_active = true;
    text_region_active = num;
    move_cursor(
        calc_col(tr_get_cursor(num)),
        calc_row(tr_get_cursor(num))
    );
}


TextRegion* tr_get(size_t num) {
    return &text_region[num];
}


uint8_t tr_get_src_col(size_t num) {
    return text_region[num].src_col;
}


uint8_t tr_get_src_row(size_t num) {
    return text_region[num].src_row;
}


uint8_t tr_get_dest_col(size_t num) {
    return text_region[num].dest_col;
}


uint8_t tr_get_dest_row(size_t num) {
    return text_region[num].dest_row;
}


uint16_t tr_get_cursor(size_t num) {
    return text_region[num].cursor_pos;
}


size_t tr_get_size() {
    return text_region_size;
}


size_t tr_get_active() {
    return text_region_active;
}