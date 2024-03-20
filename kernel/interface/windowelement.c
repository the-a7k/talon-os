#include "../include/tty.h"
#include "../include/string.h"

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


/* Work in progress */




/*
void tm_background(uint8_t color) {
    set_screen_color(color);
}


void tm_rectangle(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg) {
    for (size_t y = src_row; y <= dest_row; y++) {
        for (size_t x = src_col; x <= dest_col; x++) {
            set_cell_color(x, y, bg);  
        }
    }
}


void tm_shadow(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row) {
    tm_rectangle(src_col+1, src_row+1, dest_col+2, dest_row+1, BLACK);
}


void tm_navigation(uint8_t row, uint8_t bg, uint8_t options_fg, char* options[], size_t total_options) {
    set_row_color(row, bg);
    uint8_t current_col = 1;
    for (size_t i = 0; i < total_options; i++) {
        tm_label(options[i], current_col, row, bg, options_fg);
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


void tm_popup(uint8_t src_col, uint8_t src_row, uint8_t dest_col, uint8_t dest_row, uint8_t bg, uint8_t line_color, bool single_line) {
    tm_shadow(src_col, src_row, dest_col, dest_row);
    tm_plane(src_col, src_row, dest_col, dest_row, bg, line_color, single_line);
}


void tm_label(char *str, uint8_t col, uint8_t row, uint8_t bg, uint8_t fg) {
    // TODO: Rework label
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

void generate_sample_scene() {
    char *headers[] = {
        "Menu",
        "Options",
        "Settings",
        "Exit",
    };

    char *footers[] = {
        "F1=Help",
        "Enter=Execute",
        "Esc=Cancel",
        "Tab=Next Field",
    };
    
    char* window_text = "Welcome to talonOS === Hello, world!";
    
    tm_background(BLUE);
    tm_popup(20,6,59,10, GREEN, BLACK, false);
    tm_rectangle(2,15,35,18, BLACK);
    tm_popup(53,14,69,20, RED, PINK, false);
    tm_label(window_text, 22, 8, GREEN, WHITE);

    // Navbar
    tm_navigation(
        0, LIGHT_GREY, BLACK, headers,
        sizeof(headers) / sizeof(headers[0])
    );

    // Footer
    tm_navigation(
        ROW_SIZE - 1, CYAN, BLACK, footers, 
        sizeof(footers) / sizeof(footers[0])
    );

    create_text_region(2,15,35,18,false);
    text_region_activate(1);
}







Contents of TTY.C and TTY.H

// Specify text-writable region
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


typedef struct {
    uint8_t src_col;
    uint8_t src_row;
    uint8_t dest_col;
    uint8_t dest_row;
    uint16_t cursor_pos;
    bool is_scrollable;
    bool is_active;
} TextRegion;


#define TEXT_REGIONS_MAX 16

static TextRegion text_region[TEXT_REGIONS_MAX];
static size_t text_region_size = 0;
static size_t text_region_active = 0;


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


size_t tr_get_size() {
    return text_region_size;
}


size_t tr_get_active() {
    return text_region_active;
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


void newline() {
    TextRegion *tr = tr_get(tr_get_active());
    /*
    if (calc_row(tr->cursor_pos) >= ROW_SIZE - 1) {
        //scroll();
        return;
    }
    /*
    if (calc_row(tr->cursor_pos) < tr->dest_row) {
        move_cursor(
            tr->src_col, 
            calc_row(tr->cursor_pos) + 1);
    }
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






Contents of keyboard.c

if (key == '\b' && strlen(key_buffer) != 0) {
    size_t tr_active = tr_get_active();
    uint8_t current_col = calc_col(get_cursor_pos());
    uint8_t current_row = calc_row(get_cursor_pos());

    if (tr_get_dest_col(tr_active) == current_col && tr_get_dest_row(tr_active) == current_row) {
        if (text_region_limit) {
            cursor_retreat();
            clear_cell_cursor();
            text_region_limit = false;
        }
        else {
            clear_cell(current_col, current_row);
            text_region_limit = true;
        }
    }
    else {
        
        cursor_retreat();
        clear_cell_cursor();
    }
    strpop(key_buffer);
}

//text_region_limit = false;

*/