rm_asciiout:                    ; Main register: BX
    pusha
    str_loop:
        mov al, [bx]
        cmp al, 0               ; Check if current char is 0 (end of string)
        jne print_char
        popa
        ret

    print_char:
        mov ah, 0x0e            ; TTY mode
        int 0x10                ; Print IRQ
        inc bx                  ; Next char in variable
        jmp str_loop


rm_newline:
    mov ah, 0x0e
    mov al, 0x0a                ; ASCII line feed
    int 0x10
    mov al, 0x0d                ; ASCII carriage return
    int 0x10
    ret


rm_hexout:                      ; Main register: DX
    pusha
    mov cx, 0                   ; Counter 0-4

    hex_loop:
        cmp cx, 4               ; Fixed length of 4
        je print_hex

        mov ax, dx
        and ax, 0x000f          ; Get last number
        add ax, 0x30            ; Hex to ASCII offset
        cmp ax, 0x39            ; Compare if larger then ASCII 0-9
        jle save_hex
        add ax, 0x07            ; Convert to A-F if NaN

        save_hex:
            mov bx, HEX_STR + 5
            sub bx, cx
            mov [bx], al
            ror dx, 4           ; Rotate next number to the end
            inc cx
            jmp hex_loop

        print_hex:
            mov bx, HEX_STR
            call rm_asciiout
            popa
            ret


HEX_STR: 
    db '0x0000', 0