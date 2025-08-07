#define screen_w 80
#define screen_h 25
#include<stdint.h>
#pragma once

void EOI(int irq_num);
void int_to_intstr(int num, char* buffer);
int strncmp(const char* s1, const char* s2, int n);
uint32_t hex_to_int(const char* str);
void int_to_hex(uint32_t value, char* buffer);
void concat(char *dest, const char *src);
void serial_write_hex(int p);
unsigned int strlen(const char str[]);
unsigned int rand();
void str_to_int(char charbuff[], int intbuff[]);
