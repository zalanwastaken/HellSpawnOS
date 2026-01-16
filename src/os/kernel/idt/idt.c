#include"idt.h"
#include"../serial/serial.h"
#include"kbd/kbd.h"

IDT_entry IDT[256];
IDT_ptr IDTp;

void IDT_set_gate(uint8_t n, uint32_t base){
    IDT[n].offset_1 = base & 0xFFFF;
    IDT[n].selector = 0x08;
    IDT[n].zero = 0x0;
    IDT[n].type_attributes = 0x8E;
    IDT[n].offset_2 = (base >> 16) & 0xFFFF;
}

void EOI(uint8_t irq){
	//? 0x20 = EOI
	if(irq >= 8)
		outb(0xA0, 0x20);
	
	outb(0x20, 0x20);
}

//* handler funcs

void def_hander_C(uint32_t int_no){
    serial_print("int no.");
    serial_print_hex(int_no);
    serial_print("\n");
    while (1){
        asm volatile("hlt");
    }
}

void CPU_exept_handler_C(){
    serial_print("CPU went boom :(\n");
    while (1){
        asm volatile("hlt");
    }
}

void just_pass_C(uint32_t int_no){
    uint32_t irq = int_no - 32;
    EOI(irq);
}

//* asm funcs
//? check caller.asm

extern void def_int_pass(); //* def_handler_C
extern void CPU_exept(); //* CPU_exept_handler_C
extern void just_pass(); //* just_pass_C
extern void kbd_pass(); //* kbd_handler_C

void IDT_init(){
    for(int i = 0; i<32; i++){
        IDT_set_gate(i, (uint32_t)CPU_exept);
    }
    for(int i = 32; i<256; i++){
        IDT_set_gate(i, (uint32_t)def_int_pass);
    }
    IDT_set_gate(0x20, (uint32_t)just_pass);
    IDT_set_gate(33, (uint32_t)kbd_pass); // keyboard

    IDTp.limit = sizeof(IDT)-1;
    IDTp.base = (uint32_t)&IDT;

    init_kbd();

    asm volatile ("lidt %0" : : "m"(IDTp));
}
