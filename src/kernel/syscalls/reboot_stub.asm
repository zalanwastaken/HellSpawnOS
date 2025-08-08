global reboot

reboot:
    cli                     ; disable interrupts
    lidt [zero_idt]        ; load an empty/invalid IDT
    int 0x03               ; cause an interrupt → no handler → double fault → triple fault → REBOOT

zero_idt:
    dw 0               ; limit = 0
    dd 0               ; base = 0
