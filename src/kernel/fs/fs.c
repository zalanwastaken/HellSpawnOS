#include "fs.h"
#include "../io/serial.h"
#include "../utils.h"

void writeto(uint32_t val, int addr){
    *(uint32_t*)addr = val;
}
int readfrom(int addr){
    return(*(uint32_t*)addr);
}

void print_hex(int p){
    char buff[12];
    int_to_hex(p, buff);
    concat(buff, "\n");
    serial_write(buff);
}

volatile struct fdata *root = (volatile struct fdata*)0x0000;
int chkfileID(int id){
    volatile struct fdata *kernel_file = (volatile struct fdata*)root->data[root->namelen+1];
    if(kernel_file->id == 0xBEEF){
        return 1; // found
    }
    return 0; // Not found
}
