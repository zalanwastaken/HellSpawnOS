#define COM1 0x3F8

#include"serial.h"

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(uint16_t port, uint16_t val) {
    asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

void serial_init() {
    outb(COM1 + 1, 0x00);    // disable interrupts
    outb(COM1 + 3, 0x80);    // enable DLAB
    outb(COM1 + 0, 0x03);    // divisor low (38400 baud)
    outb(COM1 + 1, 0x00);    // divisor high
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop
    outb(COM1 + 2, 0xC7);    // enable FIFO, clear, 14-byte
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}
int serial_ready() {
    return inb(COM1 + 5) & 0x20;
}

void serial_write(char c) {
    while (!serial_ready());
    outb(COM1, c);
}

void serial_print(const char* s) {
    while (*s) serial_write(*s++);
}

void serial_print_hex(uint32_t val){
    char hex[] = "0123456789ABCDEF";
    serial_print("0x");

    for(int i = 28; i >= 0; i -= 4){
        char c = hex[(val >> i) & 0xF];
        serial_write(c);
    }
}

void serial_printLN(const char* s){
    serial_print(s);
    serial_print("\n");
}

void serial_print_hexLN(uint32_t val){
    serial_print_hex(val);
    serial_print("\n");
}
