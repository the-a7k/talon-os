[org 0x7c00]

mov bp, 0x8000
mov sp, bp
mov bx, 0x9000
mov dh, 3
call loaddisk

mov di, [0x9000]
call hexout
call newline

mov si, [0x9000 + 512]
call asciiout
call newline

mov di, [0x9000 + 1024]
call hexout
call newline

jmp $

%include "stdout.asm"
%include "disk.asm"

SEC3_STRING: 
    db 'Hello from a third sector!', 0

times 510 - ($-$$) db 0     ; HDD Sector 1 (boot: Cylinder 0, Head 0, HDD 0)
dw 0xaa55
times 256 dw 0xface         ; HDD Sector 2
times 256 dw SEC3_STRING    ; HDD Sector 3
times 256 dw 0xcaf3         ; HDD Sector 4