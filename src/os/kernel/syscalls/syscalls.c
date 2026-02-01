#include"syscalls.h"
#include"../serial/serial.h"
#include"../graphics/graphics.h"

void syscall_handler_C(regs *r){
    vbe_draw_string(0, 0, "hallo", vbe_rgb(graphicsInfo, 0, 255, 0));

    serial_print_hexLN(r->eax);
}
