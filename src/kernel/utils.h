#pragma once
#include<stdint.h>
void write_string( int colour, const char *string );
void write_string_at(int colour, const char *string, int offset);
void EOI(int irq_num);
void int_to_str(int num, char* buffer);
int strncmp(const char* s1, const char* s2, int n);
uint32_t hex_to_int(const char* str);
void int_to_hex(uint32_t value, char* buffer);
void concat(char *dest, const char *src);
