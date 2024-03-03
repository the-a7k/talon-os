[bits 32]
section .multiboot
	dd 0x1BADB002	; Magic number
	dd 0x0			; Flags
	dd - (0x1BADB002 + 0x0)	; Checksum

section .text

%include "boot/gdt.asm"


global start
global gdt_load
global idt_load
extern kernel_main			; Defined in kernel.c

gdt_load:
	lgdt [gdt_descriptor] 	; From gdt.asm
	ret


idt_load:
	mov edx, [esp + 4]
	lidt [edx]
	ret


start:
	cli
	lgdt [gdt_descriptor]
	jmp CODE_SEG:.set_cs    ; Set CS to our 32-bit flat code selector

	.set_cs:
	mov ax, DATA_SEG		; Setup the segment registers with our flat data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, stack_space	; Set stack pointer
	call kernel_main
	jmp $


section .bss
resb 8192					; 8KB for stack
stack_space: