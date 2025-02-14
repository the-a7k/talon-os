[bits 32]
section .multiboot
    dd 0x1BADB002               ; Magic number
    dd 0x0                      ; Flags
    dd - (0x1BADB002 + 0x0)     ; Checksum

section .text

%include "boot/gdt.asm"

global start
extern kernel_main

start:
    cli
    lgdt [gdt_descriptor]
    jmp CODE_SEG:.set_cs

.set_cs:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, stack_space
    push ebx                    ; Used for multiboot info
    push eax                    ; Used for multiboot info
    call kernel_main
    jmp $


section .bss
    resb 8192                   ; 8kB stack

stack_space: