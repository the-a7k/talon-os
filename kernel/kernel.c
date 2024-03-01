#include "drivers/tty.h"
#include "controller/isr.h"
#include "usermode/textview.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Fix null byte strings not working properly
// TODO: Fix cursor retreat on last character
// TODO: Rework keyboard


void kernel_main() {
    isr_setup();
    irq_setup();
    tty_setup();

    /*
    char *headers[] = {
        "Menu",
        "Options",
        "Settings",
        "Exit\0",
    };

    char *footers[] = {
        "F1=Help",
        "Enter=Execute",
        "Esc=Cancel",
        "Tab=Next Field\0",
    };


    char* window_text = "Welcome to talonOS === Hello, world!";
    create_text_region(2,15,35,18,false);
    text_region_activate(1);

    tm_background(BLUE);

    tm_window(20,6,59,10, GREEN, BLACK, false);
    tm_plane(2,15,35,18, LIGHT_CYAN, CYAN, true);
    tm_window(53,14,69,20, RED, PINK, false);
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
    */
}

