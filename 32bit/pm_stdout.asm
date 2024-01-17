[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE equ 0x0f
GREEN equ 0xa

pm_asciiout:
    pusha
    mov edx, VIDEO_MEMORY

    pm_str_loop:
        mov al, [ebx]
        mov ah, GREEN
        cmp al, 0
        jne pm_print_char
        popa
        ret

    pm_print_char:
        mov [edx], ax
        inc edx
        add ebx, 2              
        jmp pm_str_loop