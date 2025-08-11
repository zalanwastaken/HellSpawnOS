#include "idt.h"

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void idt_flush(uint32_t);  // You will write this in ASM

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].base_lo = handler & 0xFFFF;
    idt[n].base_hi = (handler >> 16) & 0xFFFF;
    idt[n].sel     = sel;
    idt[n].always0 = 0;
    idt[n].flags   = flags;
}

void idt_install() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base  = (uint32_t)&idt;

    // Zero out the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    idt_flush((uint32_t)&idtp);
}
