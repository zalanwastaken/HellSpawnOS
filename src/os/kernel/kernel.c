#include<stdint.h>

#include"serial/serial.h"
#include"graphics/graphics.h"
#include"pic/pic.h"
#include"idt/idt.h"
#include"mem_manager/manager.h"

void init(){
    IDT_init();
    PIC_init();
    mem_manager_init();

    asm volatile("sti");
}

__attribute__((section(".start")))
void kernel_main(void){
    uint32_t *kernel_size = (uint32_t*)0x7E0F;
    kernel_size[0] *= 512; //? this contains 512B sectos so now we have size in Bytes
    serial_print("Kernel size: ");
    serial_print_hex(kernel_size[0]/1024);
    serial_print("KB\n");

    init();

    for (int i = 0; i<220; i++){
        for(int f = 0; f<100; f++){
            vbe_putpixel(graphicsInfo, i, f, vbe_rgb(graphicsInfo, 255, 255, 255));
        }
    }
    vbe_draw_string_scaled(0, 0, "Hello World !", vbe_rgb(graphicsInfo, 255, 0, 0), 2);

    //TEST
    uint16_t mem = (uint16_t)kalloc(512);
    serial_print_hex(mem);
    serial_print("\n");
    kfree((void*)mem);
    serial_print_hex((uint32_t)kalloc(512));
    serial_print("\n");
    serial_print_hex((uint32_t)kalloc(512));
    serial_print("\n");

    while (1){
        asm volatile("hlt");
    }
}
