[bits 16]

gdt_start:
    dd 0x00    ; 4 byte, 32 bit
    dd 0x00    ; 4 byte, 32 bit


gdt_code:
    dw 0xffff          ; Limit                                                          - 2 byte, 16 bit
    dw 0x00            ; Base                                                           - 2 byte, 16 bit
    db 0x00            ; Base                                                           - 1 byte, 8 bit
    db 10011010b       ; Present, privilige and type bit (1001), type flags (1010)      - 1 byte, 8 bit
    db 11001111b       ; Other flags (1100), last limit bits (1111)                     - 1 byte, 8 bit
    db 0x00            ; Last base bits                                                 - 1 byte, 8 bit


gdt_data:
    dw 0xffff
    dw 0x00
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Start, 16 bit
    dd gdt_start                ; Size, 32 bit


CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start