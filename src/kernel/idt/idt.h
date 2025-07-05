#pragma once
#include <stdint.h>

struct idt_entry {
    uint16_t base_lo;   // Lower 16 bits of address
    uint16_t sel;       // Kernel segment selector
    uint8_t  always0;   // Always zero
    uint8_t  flags;     // Type + Attributes
    uint16_t base_hi;   // Upper 16 bits of address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags);
void idt_install();
