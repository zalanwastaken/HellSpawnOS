global irq80_stub
extern irq80_handler

irq80_stub:
    cli
    pushad
    mov eax, esp
    push eax
    call irq80_handler
    add esp, 4 ; idk but 4 works
    popad
    sti
    iretd
