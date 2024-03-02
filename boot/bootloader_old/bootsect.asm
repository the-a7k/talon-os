[org 0x7c00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl
mov bp, 0x9000
mov sp, bp

mov bx, RM_MSG
call rm_asciiout
call rm_newline

call enable_a20
call load_kernel
call pm_switch


%include "boot/rm_stdout.asm"
%include "boot/disk.asm"
%include "boot/pm_switch.asm"
%include "boot/gdt.asm"


[bits 16]
enable_a20:
    cli
    push ax
    mov al, 0xdd
    out 0x64, al
    pop ax       
    sti
    ret 

load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 16
    mov dl, [BOOT_DRIVE]
    call loaddisk
    ret


[bits 32]
pm_begin:
    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE: 
    db 0

RM_MSG: 
    db 'Starting...', 0


times 510 - ($-$$) db 0
dw 0xaa55