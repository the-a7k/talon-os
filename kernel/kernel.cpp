extern "C" {
    #include "include/isr.h"
    #include "include/tty.h"
    #include "include/timer.h"
    #include "include/buzzer.h"
    #include "include/keyboard.h"
    #include "include/shell.h"
}


// TODO: Add argc/argv to command


extern "C" void kernel_main();
void kernel_loop();


class CppTesting {
    //CppTesting cpptest;
    //cpptest.set_msg("Testing from C++\n");
    //cpptest.print_msg();
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
    isr_setup();
    irq_setup();
    shell_setup();
    kernel_loop();
}


void kernel_loop() {
    for (;;) {
        cpu_sleep(1);
        if (keyboard_performed_event()) {
            shell_key_handler(keyboard_get());
            if (keyboard_get()->buffer_full)
                buzzer_play(250,150);  // Handled by command, should not trigger
        }
    }
}