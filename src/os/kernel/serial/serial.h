#pragma once

#include<stdint.h>

void serial_print(const char* s);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void serial_print_hex(uint32_t val);
void serial_printLN(const char* s);
void serial_print_hexLN(uint32_t val);
