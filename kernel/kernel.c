#include "drivers/tty.h"
#include "controller/isr.h"
#include "usermode/textview.h"
#include "controller/timer.h"

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Fix null byte strings not working properly
// TODO: Fix cursor retreat on last textfield character
// TODO: Re-implement scroll
// TODO: Rework keyboard

extern void gdt_load();  // From boot/boot.asm
extern void idt_load();  // From boot/boot.asm


void kernel_main() {
    tty_setup();
    gdt_load();
    idt_load();
    isr_setup();
    irq_setup();


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
    tm_rectangle(2,15,35,18, BLACK);
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

    create_text_region(2,15,35,18,false);
    text_region_activate(1);

    kprint("This is an interrupt test!\n");
    asm volatile("int $10");
}