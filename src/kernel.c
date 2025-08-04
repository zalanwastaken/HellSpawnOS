#include "kernel/io/serial.h"
#include "kernel/utils.h"
#include "kernel/pic/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/irq/irq.h"
#include "kernel/fs/fs.h"
#include "kernel/vga/vga.h"

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

    asm volatile("int $0x80");

    clearscreen(0x000000);
    draw_string_scaled("HellSpawnOS", (800/2)-11*16, 600/2, 0xFF0000, 4);

    int kbd = findfile("root/kbd");
    if(kbd == -1){
        while (1){}
    }
    int buff[1];
    buff[0] = '\0';
    writefile(kbd, 1, buff);
    char vbuff[255];
    int vbuffs = 0;
    int y = 0;
    int x = 0;
    drawrect(x+8, y+8, x+16, y+8, 0xFFFFFF); //? draw cursor
    while (1){
        readfile(kbd, 1, buff);
        if(buff[0] != '\0'){
            drawrect(x+8, y+8, x+28, y+8, 0x000000); //? black out old cursor
            if(buff[0] == '\n'){
                y = y+8;
                x = 0;
                buff[0] = '\0';
                writefile(kbd, 1, buff);
                drawrect(x+8, y+8, x+16, y+8, 0xFFFFFF); //? draw cursor
                continue;
            }
            if(buff[0] == '\b'){
                drawrectFill(x, y, x+16, y+16, 0x000000);
                x = x - 8;
                buff[0] = '\0';
                vbuffs--;
                vbuff[vbuffs] = '\0';
                writefile(kbd, 1, buff);
                drawrect(x+8, y+8, x+16, y+8, 0xFFFFFF); //? draw cursor
                continue;
            }
            vbuff[vbuffs] = buff[0];
            vbuff[vbuffs+1] = '\0';
            x = x+8;
            draw_char(vbuff[vbuffs], x, y, 0xFFFFFF);
            drawrect(x+8, y+8, x+16, y+8, 0xFFFFFF); //? draw cursor
            vbuffs++;
            buff[0] = '\0';
            writefile(kbd, 1, buff);
        }
    }
}
