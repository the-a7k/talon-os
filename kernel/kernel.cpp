extern "C" {
    #include "drivers/tty.h"
    #include "controller/isr.h"
    #include "libc/utility.h"
    #include "libc/string.h"
}

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Rework command
// TODO: Add argc/argv to command


class CppTesting {
    private:
        char *class_msg;

    public:
        void set_msg(char *class_msg) {
            this->class_msg = class_msg;
        }
        void print_msg() { 
            kprint_color(class_msg, BLUE, YELLOW);
        }
};


extern "C" void kernel_main() {
    tty_setup();
    isr_setup();
    irq_setup();

    //generate_sample_scene();

    CppTesting test;
    test.set_msg("Testing from C++\n");
    //test.print_msg();
}