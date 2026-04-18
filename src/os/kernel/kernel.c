#include<stdint.h>

#include"graphics/graphics.h"

#include"pic/pic.h"
#include"idt/idt.h"

#include"mem_manager/manager.h"

#include"disk/disk.h"
#include"disk/fs.h"

#include"serial/logger.h"
#include"serial/serial.h"

void init(){
    mem_manager_init(0x800, 0x10000);
    graphics_init();
    IDT_init();
    PIC_init();

    asm volatile("sti");

    LOG_infoLN("Hello World !");
}

void run(){
    asm volatile("hlt");
}

__attribute__((section(".start")))
void kernel_main(void){
    init();

    uint32_t* kernel_size = kalloc(sizeof(uint32_t)*1);
    uint32_t *kernel_size_fromboot = (uint32_t*)0x7E0F;

    kernel_size_fromboot[0] *= 1024; //! dont change or stuff breaks idk why
    kernel_size[0] = kernel_size_fromboot[0];
    kernel_size_fromboot[0] = 0x00; //? we dont care about this now 

    for (int i = 0; i<220; i++){
        for(int f = 0; f<100; f++){
            vbe_putpixel(graphicsInfo, i, f, vbe_rgb(graphicsInfo, 255, 255, 255));
        }
    }
    vbe_draw_string_scaled(0, 0, "Hello World !", vbe_rgb(graphicsInfo, 255, 0, 0), 2);

    uint8_t *buff = (uint8_t*)kalloc(512);
    //serial_print_hexLN((uint32_t)buff);
    ata_read_sector(0, buff);
    kfree(buff);

    while (1){
        run();
    }
}
