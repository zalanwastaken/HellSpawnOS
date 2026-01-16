#include"pic.h"
#include"../serial/serial.h"

void pic_remap(uint32_t master_offset, uint32_t slave_offset) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, master_offset); // master offset 32
    outb(0xA1, slave_offset); // slave offset 40

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void PIC_init(){
	pic_remap(0x20, 0x28);
}
