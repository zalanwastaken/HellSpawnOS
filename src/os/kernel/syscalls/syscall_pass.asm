global syscall_pass
extern syscall_handler_C

syscall_pass:
    pusha                   ; save all registers

    mov eax, esp            ; eax = pointer to pusha frame

    push ds
    push 0x10
    pop ds

    push eax                ; argument for C
    call syscall_handler_C
    add esp, 4

    pop ds
    popa
    iret
