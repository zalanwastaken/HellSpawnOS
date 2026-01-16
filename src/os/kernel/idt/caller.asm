global def_int_pass
extern def_hander_C

global CPU_exept
extern CPU_exept_handler_C

global just_pass
extern just_pass_C

global kbd_pass
extern kbd_handler_C

CPU_exept:
    pusha
    call CPU_exept_handler_C
    popa
    iret

def_int_pass:
    pusha
    push dword 32
    call def_hander_C
    add esp, 4
    popa
    iret

just_pass:
    pusha
    push dword 32
    call just_pass_C
    add esp, 4
    popa
    iret

kbd_pass:
    pusha

    in al, 0x60        ; read scancode
    push eax
    call kbd_handler_C
    add esp, 4

    mov al, 0x20
    out 0x20, al      ; EOI to master PIC

    popa
    iret

