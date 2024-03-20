extern "C" {
    #include "include/tty.h"
    #include "include/command.h"
    #include "include/ports.h"
    #include "include/utility.h"
    #include "include/string.h"
    #include "include/isr.h"
    #include "include/timer.h"
    #include "include/speaker.h"
    #include "include/keyboard.h"
}

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Rework command
// TODO: Add argc/argv to command
// TODO: Implement ACPI


class CppTesting {
    private:
        char *class_msg;

    public:
        void set_msg(char *class_msg) {
            this->class_msg = class_msg;
        }
        void print_msg() { 
            kprint_color(class_msg, TTY_BLUE, TTY_YELLOW);
        }
};


extern "C" void kernel_main() {
    tty_setup();
    isr_setup();
    irq_setup();

    //generate_sample_scene();

    CppTesting cpptest;
    cpptest.set_msg("Testing from C++\n");
    //cpptest.print_msg();

    for (;;) {
        if (keyboard_handle_action()) {
            // TODO: Finish keyboard rework
            keyboard_t *kb = keyboard_get();
            if (kb->buffer_full)
                speaker_play(150, 50);
            if (kb->is_special)
                kprintint(kb->special_key);
            else
                kputchar(kb->last_key);
        }
    }
}