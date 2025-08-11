%macro IRQ_STUB 1
global irq%1_stub
extern irq_common_handler
irq%1_stub:
    cli
    push dword %1         ; push IRQ number as 4 bytes
    pusha
    call irq_common_handler
    popa
    add esp, 4            ; clean up pushed IRQ number
    sti
    iretd
%endmacro

IRQ_STUB 0
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15
