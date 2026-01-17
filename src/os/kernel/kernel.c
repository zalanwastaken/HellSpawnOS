#include<stdint.h>

#include"serial/serial.h"
#include"graphics/graphics.h"
#include"pic/pic.h"
#include"idt/idt.h"
#include"mem_manager/manager.h"

void init(){
    IDT_init();
    PIC_init();
    asm volatile("sti");
    mem_manager_init();
}

__attribute__((section(".start")))
void kernel_main(void){
    uint32_t *kernel_size = (uint32_t*)0x7E0F;
    kernel_size[0] *= 512; //? this contains 512B sectos so now we have size in Bytes
    serial_print("Kernel size: ");
    serial_print_hex(kernel_size[0]/1024);
    serial_print("KB\n");

    init();

    for (int i = 0; i<150; i++){
        for(int f = 0; f<100; f++){
            vbe_putpixel(graphicsInfo, i, f, vbe_rgb(graphicsInfo, 255, 255, 255));
        }
    }
    vbe_draw_string_scaled(0, 0, "Hello World !", vbe_rgb(graphicsInfo, 255, 0, 0), 2);

    //TEST

    int *mem = (int*)kalloc(515);
    kfree(mem);

    while (1){
        asm volatile("hlt");
    }
}
