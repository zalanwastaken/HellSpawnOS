#include"pit.h"
#include"../../serial/serial.h"

#include"../../scheduler/scheduler.h"

void pit_init(){
    uint16_t divisor = 1193;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void pit_handler_c(regs *r){
    scheduler_pit_catch(r);

    outb(0x20, 0x20); // send EOI
}
