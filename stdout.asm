asciiout:
    pusha
    str_loop:
        mov al, [si]
        cmp al, 0               ; Check if current char is 0 (end of string)
        jne print_char
        popa
        ret

    print_char:
        mov ah, 0x0e            ; TTY mode
        int 0x10                ; Print IRQ
        inc si                  ; Next char in variable
        jmp str_loop


newline:
    mov ah, 0x0e
    mov al, 0x0a                ; ASCII line feed
    int 0x10
    mov al, 0x0d                ; ASCII carriage return
    int 0x10
    ret


hexout:
    pusha
    mov cx, 0                   ; Counter 0-4

    hex_loop:
        cmp cx, 4               ; Fixed length of 4
        je print_hex

        mov ax, di
        and ax, 0x000f          ; Get last number
        add ax, 0x30            ; Hex to ASCII offset
        cmp ax, 0x39            ; Compare if larger then ASCII 0-9
        jle save_hex
        add ax, 0x07            ; Convert to A-F if NaN

        save_hex:
            mov si, HEX_STR + 5
            sub si, cx
            mov [si], al
            ror di, 4           ; Rotate next number to the end
            inc cx
            jmp hex_loop

        print_hex:
            mov si, HEX_STR
            call asciiout
            popa
            ret


HEX_STR: 
    db '0x0000', 0