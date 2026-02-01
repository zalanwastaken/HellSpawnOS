#include"manager.h"
#include"../serial/serial.h"

manager_Data *heap;

int ceil(double x) {
    int i = (int)x;

    if ((double)i == x)
        return i;      // already whole
    else if (x > 0)
        return i + 1;  // round up
    else
        return i;      // negatives already rounded up by cast
}

void mem_manager_init(uint32_t dataddr){
    heap = (manager_Data*)dataddr;
    size_t *kernel_size = (size_t*)0x7E0F;
    heap->block_size = 512;
    heap->startfrom = 0x1000+kernel_size[0]+0x200; //? start AFTER the kernel, leave 0x200 for data n stuff
    heap->allocatedBlocks_idx = 0;
}

void* alloc(manager_Data *data, size_t sizeToAlloc){
    size_t need = ceil((double)sizeToAlloc / data->block_size);
    uint32_t startblock = data->startfrom;
    while (1){
        bool ok = true;
        // check if we have "need" free blocks from startblock
        for(size_t b = 0; b < need; b++){
            uint32_t addr = startblock + b * data->block_size;
            for(uint32_t i = 0; i < data->allocatedBlocks_idx; i++){
                if(data->allocatedBlock[i] == addr){
                    ok = false;
                    break;
                }
            }
            if(!ok){
                break;
            };
        }
        if(ok){
            // reserve blocks
            for(size_t b = 0; b < need; b++){
                data->allocatedBlock[data->allocatedBlocks_idx++] =
                    startblock + b * data->block_size;
            }
            return (void*)startblock;
        }
        // try next block
        startblock += data->block_size;
    }
}

void free(manager_Data *data, void *toFree){
    uint32_t addr = (uint32_t)toFree;

    for(uint32_t i = 0; i < data->allocatedBlocks_idx; ){
        if(data->allocatedBlock[i] == addr){
            // shift left to delete entry
            for(uint32_t j = i; j < data->allocatedBlocks_idx - 1; j++){
                data->allocatedBlock[j] = data->allocatedBlock[j + 1];
            }
            data->allocatedBlocks_idx--;
            addr += data->block_size; // next block of same alloc
        } else {
            i++; // only advance if not deleting
        }
    }
}

void *kalloc(size_t sizeToAlloc){
    return(alloc(heap, sizeToAlloc));
}

void kfree(void* toFree){
    free(heap, toFree);
}
