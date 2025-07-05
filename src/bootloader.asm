[BITS 16]
[ORG 0x7c00]

start:
    mov ah, 0x00       ; Function: Set Video Mode
    mov al, 0x03       ; Mode 3: 80x25 text mode
    int 0x10           ; Call BIOS

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000
    mov ah, 0x02
    mov al, SECTORS
    mov ch, 0
    mov cl, 9
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc $

    cli

    xor ax, ax
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:prot_start

; --- GDT stuff ---
gdt_start:
    dq 0x0000000000000000         ; Null segment
    dq 0x00CF9A000000FFFF         ; Code segment
    dq 0x00CF92000000FFFF         ; Data segment

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_end:

[BITS 32]

prot_start:
    mov ax, 0x10      ; DATA selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    mov ax, SECTORS       ; Load SECTORS into AX
    mov [0x7E0F], ax      ; Store AX into memory at 0x7E0F
    jmp 0x1000

[BITS 16]

times 510-($-$$) db 0
dw 0xAA55
