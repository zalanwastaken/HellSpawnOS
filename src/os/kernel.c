#include "kernel/io/serial.h"
#include "kernel/io/io.h"
#include "kernel/utils.h"
#include "kernel/pic/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/irq/irq.h"
#include "kernel/fs/fs.h"
#include "kernel/vga/vga.h"
#include "kernel/tty/tty.h"

typedef void (*entry_t)(void);

void init(){
    init_serial();
    initfs();
    pic_remap();
    idt_install();
    irq_install();
    init_graphics_from_realmode_vbe();
    //ps2_mouse_init();
}

void jumpToInit(void){
    entry_t init = (entry_t)0x9000;
    init();
}

__attribute__((section(".start")))
void kernel_main(void){
    init();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("use this to debug\n");
    asm volatile("sti"); // enable interrupts

    clearscreen(0x000000);
    draw_string_scaled("HellSpawnOS", (800/2)-11*16, 600/2, 0xFF0000, 4);

    draw_string("Hello from HellSpawnOS", 0, 70, CLR_WHITE);
    //serial_write("The kernel will now panic. This is not a bug !\n");
    //panic();
    jumpToInit();
    while (1){
        asm volatile("hlt"); // just STOP
    }
}
