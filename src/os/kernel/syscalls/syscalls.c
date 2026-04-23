#include "syscalls.h"
#include "../serial/serial.h"
#include "../graphics/graphics.h"
#include "../mem_manager/manager.h"

void syscallDebugPrint(regs *r){
    serial_print("EAX: ");
    serial_print_hexLN(r->eax);
    serial_print("EBX: ");
    serial_print_hexLN(r->ebx);
    serial_print("ECX: ");
    serial_print_hexLN(r->ecx);
    serial_print("EDX: ");
    serial_print_hexLN(r->edx);
}

void syscallPrint(char *toprint){
    serial_print("SYSCALL: ");
    serial_printLN(toprint);
}

void syscall_handler_C(regs *r){
    serial_printLN("====SYSCALL====");
    syscallDebugPrint(r);
    switch (r->eax){
        default:
            syscallPrint("Un-implimented call");
            break;
    }
    serial_printLN("====SYSCALL END====");
}
