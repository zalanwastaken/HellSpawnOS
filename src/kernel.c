#include "kernel/io/serial.h"
#include "kernel/utils.h"
#include "kernel/pic/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/irq/irq.h"
#include "kernel/fs/fs.h"
#include "kernel/vga/vga.h"
#include "kernel/tty/tty.h"

void init(){
    init_serial();
    initfs();
    pic_remap();
    idt_install();
    irq_install();
    init_graphics_from_realmode_vbe();
}

__attribute__((section(".start")))
void kernel_main(void){
    init();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("use this to debug\n");
    asm volatile("sti"); // enable interrupts

    asm volatile(
        "mov $0, %eax\n"
        "mov $65, %ebx\n"
        "int $0x80"
    );

    clearscreen(0x000000);
    draw_string_scaled("HellSpawnOS", (800/2)-11*16, 600/2, 0xFF0000, 4);
    char msg[] = "Hello World !\nmeow";
    int msgint[strlen(msg)+1];
    str_to_int(msg, msgint);
    int fileptr = findfile("root/tty");
    //writefile(fileptr, strlen(msg), msgint);
    renderTTY(); // render tty

    draw_string("Hello from HellSpawnOS", 0, 60, CLR_WHITE);

    asm volatile(
        "mov $2, %eax\n"
        "int $0x80"
    );

    while (1){}
}
