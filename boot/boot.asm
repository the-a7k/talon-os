[bits 32]
section .multiboot
	dd 0x1BADB002				; Magic number
	dd 0x0						; Flags
	dd - (0x1BADB002 + 0x0)		; Checksum

section .text
global start

[extern kernel_main]			; Defined in kernel.c

start:
	cli
	mov esp, stack_space
	call kernel_main
	hlt

section .bss
resb 8192						; 8KB for stack
stack_space: