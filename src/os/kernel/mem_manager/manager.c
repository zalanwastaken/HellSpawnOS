#include"manager.h"
#include"../serial/serial.h"

manager_Data *data = (manager_Data*)0x800;
size_t *kernel_size = (size_t*)0x7E0F;

int ceil(double x) {
    int i = (int)x;

    if ((double)i == x)
        return i;      // already whole
    else if (x > 0)
        return i + 1;  // round up
    else
        return i;      // negatives already rounded up by cast
}

void mem_manager_init(){
    data->block_size = 512;
    data->startfrom = 0x1000+kernel_size[0]+0x200; //? start AFTER the kernel, leave 0x200 for data n stuff
    data->allocatedBlocks_idx = 0;
}

void* kalloc(size_t sizeToAlloc){
    size_t allocsize = ceil((double)sizeToAlloc/data->block_size); // blocks to alloc
    void* ret;
    for(uint32_t i = 0; i < allocsize; i++){
        data->allocatedBlock[data->allocatedBlocks_idx] = data->startfrom+(data->allocatedBlocks_idx*data->block_size);
        if(i == 0){
            ret = (void*)data->allocatedBlock[data->allocatedBlocks_idx];
        }
        data->allocatedBlocks_idx++;
    }
    data->allocatedBlock[data->allocatedBlocks_idx] = 0x0000; //? we start after the kernel so data here can never be 0x0, so we use these as seperators
    data->allocatedBlocks_idx++;
    return ret;
}

void kfree(void *toFree){
    uint32_t start = 0xFFFFFFFF;
    uint32_t end = 0;
    for(uint32_t i = 0; i < data->allocatedBlocks_idx; i++){
        if((void*)data->allocatedBlock[i] == toFree){
            start = i;
            break;
        }
    }
    if(start == 0xFFFFFFFF) return; // not found
    // find separator (end)
    for(uint32_t i = start; i < data->allocatedBlocks_idx; i++){
        if(data->allocatedBlock[i] == 0x0000){
            end = i;
            break;
        }
    }
    // shift everything left
    uint32_t shift = end - start + 1;
    for(uint32_t i = start; i + shift < data->allocatedBlocks_idx; i++){
        data->allocatedBlock[i] = data->allocatedBlock[i + shift];
    }
    data->allocatedBlocks_idx -= shift;
}
