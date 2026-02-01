#include<stdint.h>

#include"serial/serial.h"
#include"graphics/graphics.h"
#include"pic/pic.h"
#include"idt/idt.h"
#include"mem_manager/manager.h"
#include"disk/disk.h"

void init(){
    mem_manager_init(0x800);
    graphics_init();
    IDT_init();
    PIC_init();

    asm volatile("sti");
}

__attribute__((section(".start")))
void kernel_main(void){
    uint32_t *kernel_size = (uint32_t*)0x7E0F;
    kernel_size[0] *= 1024; //! dont change or stuff breaks idk why
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

    uint8_t *buff = (uint8_t*)kalloc(512);
    serial_print_hexLN((uint32_t)buff);
    ata_read_sector(0, buff);
    kfree(buff);

    asm volatile(
        "mov $0x10, %%eax\n"
        "int $0x80"
        :
        :
        : "eax"
    );

    while (1){
        asm volatile("hlt");
    }
}
