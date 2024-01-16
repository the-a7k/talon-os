asciiout:
    str_loop:
        mov al, [si]    ; Moves current char into 'al' register
        cmp al, 0       ; Check if current char is 0
        jne print_char  ; Jump if-not-equal
        ret             ; Function finish

    print_char:
        mov ah, 0x0e    ; Print initializer
        int 0x10        ; Print value inside of 'al' register
        inc si          ; Next char in variable
        jmp str_loop    ; Jump to next character


newline:
    mov ah, 0x0e
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10
    ret