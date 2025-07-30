#include "kernel/io/serial.h"
#include "kernel/utils.h"
#include "kernel/kbd/pic.h"
#include "kernel/idt/idt.h"
#include "kernel/kbd/irq.h"
#include "kernel/cursor/cursor.h"
#include "kernel/fs/fs.h"

void clear_low_memory() {
    unsigned char* ptr = (unsigned char*)0x0000;
    for (int i = 0; i < 0x500; i++) {
        ptr[i] = 0x00;
    }
}

void init(){
    init_serial();
    pic_remap();
    idt_install();
    irq_install();
    clear_low_memory();
    initfsroot();
    newfile(rand(), "kernel", 0x1000, (*(int*)0x7E0F)*512, 0x00FF);
    addchild(FSROOT, 0x00FF);
    newfile(rand(), "kbd", 0x7E00, 2, 0x0200);
    addchild(FSROOT, 0x0200);
}

__attribute__((section(".start")))
void kernel_main(void){
    init();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("use this to debug\n");
    asm volatile("sti"); // enable interrupts

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
        write_string_at(0x2F, "Kernel file found !", (screen_w*4)+1);
        // verifiy file
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

    //TODO wrap file read logic in fs api
    int kbd = findfile("root/kbd"); // get kbd file ptr
    if(kbd == -1){
        while(1){}
    }
    volatile struct fdata *kbdF = (volatile struct fdata*)kbd;
    char *kbdData = (char*)kbdF->ptrtodata;
    serial_write_hex(kbdF->ptrtodata);
    kbdData[0] = '\0';
    while (1) {
        if(kbdData[0] != '\0'){
            write_string(0x0F, kbdData);
            kbdData[0] = '\0';
        }
    }
}
