#include "kernel/io/serial.h"
#include "kernel/utils.h"
#include "kernel/kbd/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/kbd/irq.h"
//#include "kernel/cursor/cursor.h"
#include "kernel/fs/fs.h"
#include "kernel/vga/vga.h"
#include "kernel/vga/font.h"

void init(){
    init_serial();
    pic_remap();
    idt_install();
    irq_install();
    initfs();
    init_graphics_from_realmode_vbe();
}

void clearscreen(int color){
    for(int i = 0; i < 600; i++){
        for(int f = 0; f < 800; f++){
            putpixel(f, i, color); //! RR GG BB
        }
    }
}

__attribute__((section(".start")))
void kernel_main(void){
    init();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("use this to debug\n");
    asm volatile("sti"); // enable interrupts

    clearscreen(0x000000);
    draw_string_scaled("HellSpawnOS", (800/2)-11*16, 600/2, 0x00FF00, 4);

    int kbd = findfile("root/kbd");
    if(kbd == -1){
        while (1){}
    }
    int buff[1];
    buff[0] = '\0';
    writefile(kbd, 1, buff);
    char vbuff[255];
    int vbuffs = 0;
    while (1){
        readfile(kbd, 1, buff);
        if(buff[0] != '\0'){
            vbuff[vbuffs] = buff[0];
            vbuff[vbuffs+1] = '\0';
            draw_char(vbuff[vbuffs], vbuffs*8, 0, 0xFFFFFF);
            vbuffs++;
            buff[0] = '\0';
            writefile(kbd, 1, buff);
        }
    }
}
