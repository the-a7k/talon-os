#include "../drivers/ports.h"
#include "../drivers/screen.h"


void main() {
    clear_screen();
    print_str("Hello\nWorld!", 0, 0);
    print_str("Text Overflow", 75, 10);
    print_str("Text...", 20, 20);
    print_str("Overwrite", 24, 20);
    print_str("Row clear", 0, 79);
    clear_row(79);
}