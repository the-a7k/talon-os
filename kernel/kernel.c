#include "drivers/tty.h"
#include "controller/gdt.h"
#include "controller/isr.h"
#include "usermode/textview.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Update bootloader to GRUB (partially finished)
// TODO: Fix GDT with GRUB
// TODO: Fix IRQs with GRUB
// TODO: Fix null byte strings not working properly
// TODO: Fix cursor retreat on last textfield character
// TODO: Rework keyboard


void kernel_main() {
    tty_setup();
    isr_setup();
    //irq_setup();

    kprint_color("Hello, world..?", BLACK, CYAN);

    /*
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

