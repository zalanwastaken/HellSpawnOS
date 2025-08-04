#include "../io/serial.h"
#include "../utils.h"

void irq80_handler(){
    serial_write("yay\n");
    EOI(0x80);
}
