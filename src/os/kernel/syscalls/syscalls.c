#define SYSCALL_TTY_W 0
#define SYSCALL_TTY_RENDER 1
#define SYSCALL_KPANIC 2
#define SYSCALL_PRINT_AT 3

extern void reboot();

#include "../io/serial.h"
#include "../utils.h"
#include "../fs/fs.h"
#include "../vga/vga.h"
#include "../io/io.h"
#include "../tty/tty.h"

typedef struct regs{
    int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
} regs;

void syscallDebug(regs *registers){
    serial_write("EAX: ");
    serial_write_hex(registers->EAX); // this adds \n new line
    serial_write("EBX: ");
    serial_write_hex(registers->EBX); // this adds \n new line
    serial_write("ECX: ");
    serial_write_hex(registers->ECX); // this adds \n new line
    serial_write("EDX: ");
    serial_write_hex(registers->EDX); // this adds \n new line
    serial_write("EBP: ");
    serial_write_hex(registers->EBP);
}

void dispatch(regs *registers){
    if(registers->EAX == SYSCALL_TTY_W){
        int wri[] = {registers->EBX, '\0'};
        int file = findfile("root/tty");
        if(file == -1){
            panic(0x02);
        }
        writefile(file, 2, wri);
    }

    if(registers->EAX == SYSCALL_TTY_RENDER){
        renderTTY();
    }

    if(registers->EAX == SYSCALL_KPANIC){
        serial_write("---- KERNEL PANIC ----\n");
        serial_write("SYSCALL TRIGGERED PANIC\n");
        drawrectFill(0, 0,27*11, 60, CLR_WHITE);

        draw_string_scaled("KERNEL PANIC", 0, 0, CLR_RED, 3);
        draw_string("SYSCALL TRIGGERED PANIC", 0, 27, CLR_RED);
        char buff[12];
        int_to_hex(registers->EBX, buff);
        draw_string("CODE: ", 0, 28+8, CLR_RED);
        draw_string(buff, 8*5, 28+8, CLR_RED);
        draw_string("F1: REBOOT", 0, 32+16, CLR_BLUE);

        int kbd = inb(0x60); //NOTE: kbd api wont work as we are in a int currently so we are checking for kbd input here
        while (1){
            kbd = inb(0x60);
            if(kbd == 0x3B){
                serial_write("REBOOT\n");
                reboot();
            }
        }
    }

    if(registers->EAX == SYSCALL_PRINT_AT){
        int color = registers->EBX;
        int x = registers->ECX;
        int y = registers->EDX;
        char *str = (char*)registers->ESI; // assume its \0 terminated
        draw_string(str, x, y, color);
    }
}

void irq80_handler(regs *r){
    serial_write("---- SYSCALL ----\n");
    syscallDebug(r);
    dispatch(r);
    serial_write("---- END OF SYSCALL ----\n");
    EOI(0x80);
}
