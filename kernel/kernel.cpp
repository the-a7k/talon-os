extern "C" {
    #include "include/tty.h"
    #include "include/command.h"
    #include "include/isr.h"
    #include "include/speaker.h"
    #include "include/keyboard.h"
    #include "include/timer.h"
}

/* C libraries used: stdint.h, stddef.h, stdbool.h */

// TODO: Add argc/argv to command
// TODO: Implement ACPI(?)

extern "C" void kernel_main();
void kernel_loop();


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
    //CppTesting cpptest;
    //cpptest.set_msg("Testing from C++\n");
    //cpptest.print_msg();
    kernel_loop();
}


void kernel_loop() {
    for (;;) {
        cpu_sleep(1);

        if (keyboard_performed_event()) {
            command_key_handler(keyboard_get());
            if (keyboard_get()->buffer_full)
                speaker_play(250,150);  // Handled by command, should not trigger
        }
    }
}