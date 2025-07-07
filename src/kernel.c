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
}

__attribute__((section(".start")))
void kernel_main(void){
    clear_low_memory();
    initfsroot();
    newfile(0xBEEF, "kernel", 0x1000, (*(int*)0x7E0F)*512, 0x00FF);
    addchild(FSROOT, 0x00FF);

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

    if(chkfileID(0xBEEF) == 1){
        write_string_at(0x0F, "Kernel file found !", (screen_w*4)+1);

        volatile struct fdata* root = (volatile struct fdata*)FSROOT;
        // verifiy file
        volatile struct fdata* kernel = (volatile struct fdata*)root->data[root->namelen+1];
        char name[kernel->namelen];
        for(int i = 0; i <= kernel->namelen; i++){
            name[i] = kernel->data[i];
        }
        write_string_at(0x0F, name, (screen_w*5)+1);
    }else{
        write_string_at(0x0F, "Kernel file not found !", (screen_w*4)+1);
    }

    serial_write("reached end of test !\n");

    while (1) {}
}
