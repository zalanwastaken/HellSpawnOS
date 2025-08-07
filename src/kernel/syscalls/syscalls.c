#define SYSCALL_TTY_W 0
#define SYSCALL_TTY_R 1 // NOT IMPLIMENTED
#define SYSCALL_KPANIC 2

#include "../io/serial.h"
#include "../utils.h"
#include "../fs/fs.h"
#include "../vga/vga.h"

typedef struct regs{
    int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
} regs;

void syscallDebug(regs *registers){
    serial_write("---- SYSCALL ----\n");
    serial_write("EAX: ");
    serial_write_hex(registers->EAX); // this adds \n new line
    serial_write("EBX: ");
    serial_write_hex(registers->EBX); // this adds \n new line
    serial_write("ECX: ");
    serial_write_hex(registers->ECX); // this adds \n new line
    serial_write("EDX: ");
    serial_write_hex(registers->EDX); // this adds \n new line
    serial_write("---- END OF SYSCALL ----\n");
}

void dispatch(regs *registers){
    syscallDebug(registers);

    if(registers->EAX == SYSCALL_TTY_W){
        int wri[] = {registers->EBX, '\0'};
        int file = findfile("root/tty");
        writefile(file, 2, wri);
    }

    if(registers->EAX == SYSCALL_KPANIC){
        serial_write("---- KERNEL PANIC ----\n");
        serial_write("SYSCALL TRIGGRED PANIC\n");
        drawrectFill(0, 0,27*11, 40, CLR_WHITE);
        draw_string_scaled("KERNEL PANIC", 0, 0, CLR_RED, 3);
        draw_string("SYSCALL TRIGGRED PANIC", 0, 27, CLR_RED);
        while (1){
            asm volatile("hlt");
        }
    }
}

void irq80_handler(regs *r){
    dispatch(r);

    EOI(0x80);
}
