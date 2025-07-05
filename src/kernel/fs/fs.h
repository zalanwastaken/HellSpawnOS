#include<stdint.h>
#pragma once
int chkfileID(int id);
void writeto(uint32_t val, int addr);
void print_hex(int p);
struct fdata{
    int isdir;
    int id;
    int namelen;
    int childlenORfilesize;
    int ptrtodata;
    int data[];
};
