gdt_start:

gdt_null:
    dd 0x0              ; 4 byte, 32 bit
    dd 0x0              ; 4 byte, 32 bit

gdt_code:
    dw 0xffff           ; Limit                                                          - 2 byte
    dw 0x0000           ; Base                                                           - 2 byte
    db 0x00             ; Base                                                           - 1 byte
    db 10011010b        ; Present, privilige and type bit (1001), type flags (1010)      - 1 byte
    db 11001111b        ; Other flags (1100), last limit bits (1111)                     - 1 byte
    db 0x00             ; Last base bits

gdt_data:
    dw 0xfffff
    dw 0x0000	
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1          ; Start, 16 bit
    dd gdt_start                        ; Size, 32 bit

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start