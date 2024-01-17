loaddisk:
    pusha
    push dx

    mov ah, 2       ; Read initialization

    mov al, dh      ; HDD sectors to read
    mov cl, 2       ; HDD start sector number
    mov ch, 0       ; HDD cylinder number
    mov dh, 0       ; HDD head number

    int 0x13        ; Disk access IRQ
    jc disk_err

    pop dx
    cmp al, dh
    jne sector_err
    popa
    ret

    disk_err:
        mov si, DISK_ERROR
        call rm_asciiout
        mov dh, ah
        call rm_newline
        call rm_hexout
        jmp disk_loop

    sector_err:
        mov si, SECTORS_ERROR
        call rm_asciiout

    disk_loop:
        jmp $


DISK_ERROR: 
    db "Disk read error", 0
    
SECTORS_ERROR: 
    db "Incorrect number of sectors read", 0