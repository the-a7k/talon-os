[bits 16]

pm_switch:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax                ; Entering protected mode
    jmp CODE_SEG:pm_start       ; Long jump


[bits 32]

pm_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    call PM_BEGIN