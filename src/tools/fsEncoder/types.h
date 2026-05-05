#pragma once

#include<stdint.h>
#include<stddef.h>

typedef struct __attribute__((packed)){
    bool alwaysTRUE; //NOTE this is magic bool, this should always be true in a file
    char name[128];
    size_t datalen;
    uint32_t *data;
    uint32_t dataoffset;
}file_t;

typedef struct __attribute__((packed)){
    bool alwaysFALSE; //NOTE this is magic bool, this should always be false in a dir
    char name[128];
    bool isroot;
    uint8_t children_amt;
    uint32_t childrenoffsets[256];
}dir_t;

typedef struct __attribute__((packed)){
    uint8_t always111; //NOTE this is magic int, this should always be 111 in this
    uint32_t dirblocksize;
    uint32_t fileblocksize;
}aligner_t;
