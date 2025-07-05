#include "irq.h"
#include "../idt/idt.h"
#include "../io/serial.h"
#include "../io/io.h"
#include "../utils.h"

// Define all IRQ stubs (implemented in IRQ ASM file)
extern void irq0_stub();
extern void irq1_stub();
extern void irq2_stub();
extern void irq3_stub();
extern void irq4_stub();
extern void irq5_stub();
extern void irq6_stub();
extern void irq7_stub();
extern void irq8_stub();
extern void irq9_stub();
extern void irq10_stub();
extern void irq11_stub();
extern void irq12_stub();
extern void irq13_stub();
extern void irq14_stub();
extern void irq15_stub();

// Simple common IRQ handler
void irq_common_handler(int irq_num) {
    if(irq_num != 0){ // check for PIT
        char msg[] = "IRQX\n";
        msg[3] = '0' + irq_num;  // just shows 0–9, extend if needed
        serial_write(msg);
    }

    // End of Interrupt
    EOI(irq_num);
}

void irq_install() {
    idt_set_gate(32, (uint32_t)irq0_stub,  0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1_stub,  0x08, 0x8E); // IRQ1 = keyboard
    idt_set_gate(34, (uint32_t)irq2_stub,  0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3_stub,  0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4_stub,  0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5_stub,  0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6_stub,  0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7_stub,  0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8_stub,  0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9_stub,  0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10_stub, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11_stub, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12_stub, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13_stub, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14_stub, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15_stub, 0x08, 0x8E);
}
