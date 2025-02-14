extern "C" {
    #include "include/isr.h"
    #include "include/tty.h"
    #include "include/timer.h"
    #include "include/buzzer.h"
    #include "include/keyboard.h"
    #include "include/shell.h"
    #include "include/string.h"
    #include "include/multiboot.h"
}

class CppTesting {
    /*
        Testing C/C++ compatibility

        CppTesting cpptest;
        cpptest.set_msg("Testing from C++\n");
        cpptest.print_msg();
    */
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


void kernel_loop() {
    for (;;) {
        cpu_sleep(1);                       // Halting the CPU each second
        if (keyboard_performed_event()) {
            shell_key_handler();            // Handle keyboard buffer, print output, process commands...
            if (keyboard_buffer_full())     
                buzzer_play(250,200);       // Handled by shell, should not trigger
        }
    }
}


extern "C" void kernel_main(uint32_t magic, struct multiboot_info *boot_info) {
    isr_setup();
    irq_setup();
    shell_setup();
    kernel_loop();
}