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
        case 1: //* WRITE
            if(r->ebx == 1){
                char *buff = (char*)r->ecx;
                char *mybuff = kalloc(sizeof(char)*(r->edx+1));
                for(uint32_t i = 0; i<r->edx; i++){
                    mybuff[i] = buff[i];
                }
                mybuff[r->edx] = '\0'; // just to be safe
                serial_print(mybuff);
                kfree(mybuff);
                r->eax = r->edx;
            }else{
                syscallPrint("NON-STDOUT fd found good luck bro");
                r->eax = -38;
            }
            break;
        case 2: //* READ
            r->eax = 0;
            break;

        case 3: //* OPEN
            r->eax = -2;
            break;

        case 7: //* EXIT
            syscallPrint("Some proc exited");
            while (1){
                asm volatile("hlt");
            }
            break;
        
        case 10: //* ALLOC MEM
            void *ptr = kalloc(r->ebx);
            r->eax = (uint32_t)ptr;
            break;

        case 11: //* FREE MEM
            kfree((void*)r->ebx);
            break;
        
        default:
            syscallPrint("Un-implimented call");
            break;
    }
    serial_printLN("====SYSCALL END====");
}
