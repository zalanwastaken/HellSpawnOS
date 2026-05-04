#include<stdint.h>
#include<stddef.h>

#include"graphics/graphics.h"

#include"pic/pic.h"
#include"idt/idt.h"

#include"mem_manager/manager.h"

#include"disk/disk.h"
#include"disk/fs.h"

#include"serial/logger.h"
#include"serial/serial.h"

#include"idt/kbd/kbd.h"

void init(){
    mem_manager_init(0x100, 0x10000);
    graphics_init();
    LOG_init();
    IDT_init();
    PIC_init();
    FS_init();

    asm volatile("sti"); //? enable ints
}

bool release = true;

void run(){
    char *keyPressed = kbd_getpressed();
    if(keyPressed[0] == 'd' && release){
        manager_Data *heapData = getHeapData(); //! LIVE POINTER DO NOT MODIFY!
        manager_Data *copiedData = (manager_Data*)kalloc(sizeof(manager_Data)+(sizeof(uint32_t)*heapData->allocatedBlocks_idx));
        memcopy(copiedData, heapData, sizeof(manager_Data)+(sizeof(uint32_t)*heapData->allocatedBlocks_idx));
        heapData = NULL; //* Try to modify it now >:)
        uint16_t heapsize = (copiedData->allocatedBlocks_idx*copiedData->block_size);
        kfree(copiedData);

        char *toprint = (char*)kalloc(sizeof(char)*256);

        LOG_format(toprint, sizeof(char)*256, "Heap size is %dB", heapsize);
        LOG_infoLN(toprint);

        kfree(toprint);
        release = false;

        uint32_t *logged_IDX_arg = 0;
        char *logged_msgs = LOG_get_logged(logged_IDX_arg);
        uint32_t logged_IDX = logged_IDX_arg[0];
        LOG_HexLN(LOG_GREEN, "[HEX] ", logged_IDX);

        vbe_rectangle(0, 0, 400, 400, vbe_rgb(graphicsInfo, 0, 0, 0));

        vbe_draw_string_scaled(0, 0, "HELLSPAWN OS", vbe_rgb(graphicsInfo, 255, 0, 0), 2.5);
        vbe_draw_string(0, 15, "Debug info", vbe_rgb(graphicsInfo, 255, 128, 0));

        uint32_t idx_off = 0;
        for(uint32_t i = 0; i<logged_IDX; i++){
            uint32_t idx = 0;
            char buff[256];
            while(true){
                buff[idx] = logged_msgs[idx+idx_off];
                if(logged_msgs[idx+idx_off] == '\0'){
                    idx_off += idx+1;
                    break;
                }
                idx++;
            }
            vbe_draw_string_scaled(0, 30+(i*15), (const char*)buff, vbe_rgb(graphicsInfo, 255, 255, 0), 2);
        }

    }else if(keyPressed[0] != 'd'){
        release = true;
    }
}

__attribute__((section(".start")))
void kernel_main(void){
    init();

    uint32_t* kernel_size = kalloc(sizeof(uint32_t)*1);
    uint32_t *kernel_size_fromboot = (uint32_t*)0x7E0F;

    kernel_size_fromboot[0] *= 1024; //! dont change or stuff breaks idk why
    kernel_size[0] = kernel_size_fromboot[0];
    kernel_size_fromboot[0] = 0x00; //? we dont care about this now

    vbe_rectangle(0, 0, 300, 200, vbe_rgb(graphicsInfo, 255, 255, 255));
    vbe_draw_string_scaled(0, 0, "Hello World !", vbe_rgb(graphicsInfo, 255, 0, 0), 2);

    uint8_t *buff = (uint8_t*)kalloc(512); //! 8 bits to hold that ptr, not good !
    //serial_print_hexLN((uint32_t)buff);
    ata_read_sector_safe(0, buff);
    kfree(buff);

    LOG_infoLN("Welcome to HellSpawn OS!");

    while (1){
        run();
    }
}
