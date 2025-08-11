global irq44_stub
extern irq44_handler

irq44_stub:
    cli
    pusha
    call irq44_handler
    popa
    sti
    iretd
