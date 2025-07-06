#include "fs.h"
#include "../io/serial.h"
#include "../utils.h"

volatile struct fdata *root = (volatile struct fdata*)0x0000;

void initfsroot(){
    root->isdir = 1; // this is a dir(root)
    root->id = 0xDEAD;
    root->namelen = 3; // starting from 0
    for(int i = 0; i <= root->namelen; i++){
        char name[4] = "root";
        root->data[i] = name[i]; // set its name
    }
    root->childlenORfilesize = 0;
}

int chkfileID(int id){
    volatile struct fdata *kernel_file = (volatile struct fdata*)root->data[root->namelen+1];
    if(kernel_file->id == id){
        return 1; // found
    }
    return 0; // Not found
}

void newfile(int id, const char name[], int dataloc, int size, int location){
    volatile struct fdata *ret = (volatile struct fdata*)location;
    ret->isdir = 0;
    ret->id = id;
    ret->namelen = strlen(name);
    ret->ptrtodata = dataloc;
    ret->childlenORfilesize = size;
    for(int i = 0; i <= ret->namelen; i++){
        ret->data[i] = name[i];
    }
}

void addchild(int parent_location, int child_location){
    volatile struct fdata *parent = (volatile struct fdata*)parent_location;
    if(parent->isdir == 1){
        parent->childlenORfilesize++;
        parent->data[parent->namelen+parent->childlenORfilesize] = child_location;
    }
}
