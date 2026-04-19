#pragma once

#include<stdint.h>

struct DIR{
    char name[255];
    //bool isroot;
    uint64_t children_amt;
    uint32_t *children[];
} typedef DIR_T;

struct FILE{
    char name[255];
    //size_t content_size;
    void *content[];
} typedef FILE_T;


void fs_init();
