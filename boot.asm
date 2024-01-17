[org 0x7c00]

mov bp, 0x9000
mov sp, bp

mov si, RM_MSG
call rm_asciiout
call rm_newline
mov si, RM_NOTICE
call rm_asciiout

call pm_switch

%include "16bit/rm_stdout.asm"
%include "16bit/disk.asm"
%include "32bit/pm_stdout.asm"
%include "32bit/pm_switch.asm"
%include "32bit/gdt.asm"

[bits 32]
PM_BEGIN:
    mov edx, PM_MSG
    call pm_asciiout
    jmp $


RM_MSG: 
    db 'Entered real mode. Loading protected mode...', 0

RM_NOTICE: 
    db 'On load success, top-left character should be GREEN.', 0

PM_MSG: 
    db 'Entering protected mode.', 0   


times 510 - ($-$$) db 0
dw 0xaa55