#pragma once
#include<stdint.h>
#include<stddef.h> // for size_t
#include<stdbool.h>

typedef struct __attribute__((packed)){
    size_t block_size;
    uint32_t startfrom;
    uint32_t allocatedBlocks_idx;
    uint32_t allocatedBlock[];
}manager_Data;

void mem_manager_init();
void* kalloc(size_t sizeToAlloc);
void kfree(void *toFree);
