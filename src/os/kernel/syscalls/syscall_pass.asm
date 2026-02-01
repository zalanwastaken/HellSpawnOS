global syscall_pass
extern syscall_handler_C

syscall_pass:
    ;cli
    pusha           ; push AX, CX, DX, BX, SP, BP, SI, DI

    push ds
    push 0x10
    pop ds

    mov eax, esp    ; pointer to pushed registers (pusha order)
    push eax        ; push pointer for cdecl call
    call syscall_handler_C
    add esp, 4      ; cleanup

    pop ds
    popa
    ;sti
    iret
