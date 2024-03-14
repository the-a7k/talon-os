extern "C" {
    #include "drivers/tty.h"
    #include "controller/isr.h"
    #include "interface/windowelement.h"
}

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Re-implement scroll
// TODO: Space removes char on TextRegion limit
// TODO: Rework keyboard
// TODO: Begin migration to C++ (?)
// TODO: Add a window manager


class CppTesting {
    private:
        char *class_msg;

    public:
        char *set_msg(char *class_msg) {
            this->class_msg = class_msg;
        }
        void print_msg() { 
            kprint(class_msg);
        }
};


extern "C" void kernel_main() {
    tty_setup();
    isr_setup();
    irq_setup();

    generate_sample_scene();

    CppTesting test;
    test.set_msg("Testing from C++\n");
    test.print_msg();
}