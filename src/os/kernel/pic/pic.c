#include "pic.h"
#include "../io/io.h"

void pic_remap() {
    outb(0x20, 0x11);  // start init
    outb(0xA0, 0x11);

    outb(0x21, 0x20);  // remap master IRQs to 0x20–0x27
    outb(0xA1, 0x28);  // remap slave  IRQs to 0x28–0x2F

    outb(0x21, 0x04);  // tell Master PIC there is a slave at IRQ2 (0000 0100)
    outb(0xA1, 0x02);  // tell Slave PIC its cascade identity (0000 0010)

    outb(0x21, 0x01);  // 8086 mode
    outb(0xA1, 0x01);

    outb(0x21, 0x0);   // unmask
    outb(0xA1, 0x0);
}
