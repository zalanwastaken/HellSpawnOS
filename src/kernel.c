#define screen_w 80
#define screen_h 25

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

__attribute__((section(".start")))
void kernel_main(void){
    clear_low_memory();
    // root
    volatile struct fdata *root = (volatile struct fdata*)0x0000;
    root->isdir = 1; // this is a dir(root)
    root->id = 0xDEAD;
    root->namelen = 3; // starting from 0
    for(int i = 0; i <= root->namelen; i++){
        char name[4] = "root";
        root->data[i] = name[i]; // set its name
    }
    root->childlenORfilesize = 1;
    root->data[root->namelen+1] = 0x00FF;

    volatile struct fdata *kernel_file = (volatile struct fdata*)0x00FF;
    kernel_file->id = 0xBEEF;
    kernel_file->isdir = 0;
    kernel_file->ptrtodata = 0x1000-0x0006;
    kernel_file->namelen = 5;

    for(int i = 0; i <= kernel_file->namelen; i++){
        char name[] = "kernel";
        kernel_file->data[i] = name[i];
    }

    init_serial();
    serial_write("HELLO FROM HELLSPAWNOS\n");
    serial_write("USE THIS TO DEBUG\n");
    pic_remap();
    idt_install();
    irq_install();
    asm volatile("sti");

    int posfornextchar = 0;
    char* kbd_inp = (char*)0x7E00;
    char buff[12];
    int_to_hex(0x1000, buff);
    write_string_at(0x0F, "Kernel at", 1);
    write_string_at(0x0F, buff, screen_w+1);
    int_to_hex((*(int*)0x7e0f)*512, buff);
    write_string_at(0x0F, "Kernel size", (screen_w*2)+1);
    write_string_at(0x0F, buff, (screen_w*3)+1);
    if(chkfileID(0xAAAA) == 1){
        write_string_at(0x0F, "Kernel file found !", (screen_w*4)+1);
    }else{
        write_string_at(0x0F, "Kernel file not found !", (screen_w*4)+1);
    }

    //posfornextchar += screen_w - (posfornextchar % screen_w);
    //move_cursor(posfornextchar);
    while (1) {
        /*
        if(kbd_inp[0] == '\b' && posfornextchar != 0){
            posfornextchar--;
            write_string_at(0x0F, " ", posfornextchar);
            move_cursor(posfornextchar);
            kbd_inp[0] = 0x00;
            continue;
        }
        if (kbd_inp[0] != 0x00 && kbd_inp[0] != '\b' && kbd_inp[0] != '\n'){
            write_string_at(0x0F, kbd_inp, posfornextchar);
            posfornextchar++;
            move_cursor(posfornextchar);
            kbd_inp[0] = 0x00;
            continue;
        }
        */ //disabled for now
    }
}
