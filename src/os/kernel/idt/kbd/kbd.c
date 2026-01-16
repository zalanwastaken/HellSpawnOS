#include"kbd.h"
#include"../../serial/serial.h"

void init_kbd(){
    outb(0x21, 0xFD); // 11111101 → enable IRQ1 (keyboard)
    outb(0xA1, 0xFF); // disable all slave IRQs
}
