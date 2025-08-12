global reboot

reboot:
    cli
    mov al, 0xFE          ; pulse CPU reset line
    out 0x64, al          ; write to PS/2 keyboard controller command port
.hang:
    hlt                   ; in case it doesn’t reboot instantly
    jmp .hang
