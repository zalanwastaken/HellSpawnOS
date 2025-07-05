#include <stdint.h>
void newprecarray(int addr, int len, int buff[]){
    buff[0] = addr;
    //buff[1] = len;
}
void set32bval(int precarray[], int idx, uint32_t val){
    uint32_t *target = (uint32_t*)precarray[0]+idx;
    *target = val;
}
