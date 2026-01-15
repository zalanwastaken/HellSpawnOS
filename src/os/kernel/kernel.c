#include<stdint.h>

#include"serial/serial.h"
#include"graphics/graphics.h"

__attribute__((section(".start")))
void kernel_main(void){
    serial_print("Hello World !");
    for (int i = 0; i<100; i++){
        for(int f = 0; f<100; f++){
            vbe_putpixel(graphicsInfo, i, f, vbe_rgb(graphicsInfo, 255, 255, 255));
        }
    }
    while (1){
        asm volatile("hlt"); // just STOP
    }
}
