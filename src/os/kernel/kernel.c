#include<stdint.h>

#include"serial/serial.h"
#include"graphics/graphics.h"
#include"pic/pic.h"
#include"idt/idt.h"

void init(){
    IDT_init();
    PIC_init();
    asm volatile("sti");
}

__attribute__((section(".start")))
void kernel_main(void){
    uint32_t *kernel_size = (uint32_t*)0x7E0F;
    kernel_size[0] *= 512; //? this contains 512B sectos so now we have size in Bytes
    serial_print("Hello World !\nKernel size: ");
    serial_print_hex(kernel_size[0]/1024);
    serial_print("KB\n");

    init();

    for (int i = 0; i<100; i++){
        for(int f = 0; f<100; f++){
            vbe_putpixel(graphicsInfo, i, f, vbe_rgb(graphicsInfo, 255, 0, 255));
        }
    }

    while (1){
        asm volatile("hlt"); // just STOP
    }
}
