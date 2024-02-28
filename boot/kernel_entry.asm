[bits 32]
[extern kernel_main]   ; Linker magic
call kernel_main
jmp $