#include "kernel/io/serial.h"
#include "kernel/utils.h"
#include "kernel/kbd/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/kbd/irq.h"
//#include "kernel/cursor/cursor.h"
#include "kernel/fs/fs.h"
#include "kernel/vga/vga.h"

void init(){
    init_serial();
    pic_remap();
    idt_install();
    irq_install();
    initfs();
}

__attribute__((section(".start")))
void kernel_main(void){
    init();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("use this to debug\n");
    asm volatile("sti"); // enable interrupts

    //serial_write("starting self-FS test\n");
    init_graphics_from_realmode_vbe();
    for(int i = 0; i < 600; i++){
        for(int f = 0; f < 800; f++){
            putpixel(f, i, 0xF0F0F0F0);
        }
    }

    /*
    char buff[12];
    int_to_hex(0x1000, buff);
    write_string_at(0x0F, "Kernel at", 1);
    write_string_at(0x0F, buff, screen_w+1);
    int_to_hex((*(int*)0x7e0f)*512, buff);
    write_string_at(0x0F, "Kernel size", (screen_w*2)+1);
    write_string_at(0x0F, buff, (screen_w*3)+1);

    int kernelF = findfile("root/kernel");
    serial_write_hex(kernelF);
    if(kernelF != -1){
        write_string_at(0x2F, "Kernel file found !", (screen_w*4)+1); // sad
        //* verifiy file
        volatile struct fdata* kernel = (volatile struct fdata*)kernelF;
        char name[kernel->namelen+1];
        for(int i = 0; i <= kernel->namelen; i++){
            name[i] = kernel->data[i];
        }
        name[kernel->namelen+1] = '\0';
        write_string_at(0x2F, name, (screen_w*5)+1); //! give visiable verification
    }else{
        write_string_at(0x4F, "Kernel file not found !", (screen_w*4)+1);
    }

    serial_write("reached end of test !\n");

    int kbd = findfile("root/kbd"); // get kbd file ptr
    if(kbd == -1){ //? check for fail
        while(1){}
    }
    int kbdData[1];
    readfile(kbd, 1, kbdData);
    while (1) {
        readfile(kbd, 1, kbdData);
        if(kbdData[0] != '\0'){
            write_string(0x0F, (char*)kbdData);
            int buff[1];
            readfile(kbd, 1, buff);
            buff[0] = '\0';
            writefile(kbd, 1, buff);
        }
    }
    */
    while (1){
    }
}
