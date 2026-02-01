global syscall_pass
extern syscall_handler_C

syscall_pass:
    pusha                   ; save all registers
    push ds
    push 0x10
    pop ds

    mov eax, esp            ; pointer to pusha frame
    push eax
    call syscall_handler_C
    add esp, 4              ; cleanup pointer

    pop ds
    popa
    iret
