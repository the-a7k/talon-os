[org 0x7c00]

mov si, STRING_ONE
call asciiout
call newline
mov si, STRING_TWO
call asciiout

jmp $

%include "stdout.asm"
STRING_ONE: db 'Testing testing...', 0
STRING_TWO: db 'Hello!', 0


times 510 - ($-$$) db 0
dw 0xaa55