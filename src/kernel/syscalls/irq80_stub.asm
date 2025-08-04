global irq80_stub
extern irq80_handler

irq80_stub:
    pusha
    call irq80_handler
    popa
    iretd
