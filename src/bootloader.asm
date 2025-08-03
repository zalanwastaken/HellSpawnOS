[BITS 16]
[ORG 0x7c00]

start:
    mov ax, 0x4F02
    mov bx, 0x4118
    int 0x10
    cmp ax, 0x004F
    jne vga_err
    mov ax, 0
    mov es, ax            ; using identity-mapped low memory
    mov ax, 0x4F01        ; VBE: Get Mode Info
    mov cx, 0x118         ; mode number
    mov di, 0x900         ; buffer at 0x0000:0x0900 (256-byte, 16-byte aligned)
    int 0x10

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
    jc disk_err

    call enable_a20

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

;; --- 32 BIT land ---
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

;; --- 16 BIT functions ---

[BITS 16]
beep:
    ; Set PIT channel 2 to square wave mode (mode 3)
    mov al, 0b10110110         ; Channel 2, lobyte/hibyte, mode 3, binary
    out 0x43, al

    ; Set frequency (e.g., 1000 Hz = 1193182 / 1000 ≈ 1193)
    mov ax, 1193
    out 0x42, al               ; Send low byte
    mov al, ah
    out 0x42, al               ; Send high byte

    ; Enable speaker (set bits 0 and 1 in port 0x61)
    in al, 0x61
    or al, 0b00000011
    out 0x61, al

    ; Delay loop
    mov cx, 0xFFFF
    .delay_loop:
        loop .delay_loop
    mov cx, 0xFFFF
    .delay_loop2:
        loop .delay_loop2
        ; Disable speaker (clear bits 0 and 1)
        in al, 0x61
        and al, 0b11111100
        out 0x61, al
    ret

enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

disk_err:
    call beep
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    mov al, '1'
    int 0x10
    jmp $

vga_err:
    call beep
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    mov al, '2'
    int 0x10
    jmp $

;; --- Magic number ---

times 510-($-$$) db 0
dw 0xAA55
