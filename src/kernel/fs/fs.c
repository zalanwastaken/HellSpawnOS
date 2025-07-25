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

//TODO remove
int chkfileID(int id){
    volatile struct fdata *kernel_file = (volatile struct fdata*)root->data[root->namelen+1];
    if(kernel_file->id == id){
        return 1; // found
    }
    return 0; // Not found
}

void parsepath(const char path[], char buff[], unsigned int dirN) {
    int i = 0;  // Index in path
    int count = 0; // How many directories we've seen
    int j = 0;  // Index in buff

    // Skip leading slashes
    while (path[i] == '/') i++;

    while (path[i] != '\0') {
        if (count == dirN) {
            // Start copying into buff
            while (path[i] != '/' && path[i] != '\0') {
                buff[j++] = path[i++];
            }
            break;
        }

        // Skip this directory
        while (path[i] != '/' && path[i] != '\0') i++;
        // Skip subsequent slashes
        while (path[i] == '/') i++;
        count++;
    }

    buff[j] = '\0'; // Null terminate

    // If dirN was out of range, return empty string
    if (count < dirN) {
        buff[0] = '\0';
    }
}

int findfile(char path[]){
    volatile struct fdata *root = (volatile struct fdata*)FSROOT;
    char buff[32];
    for(int i = 0; i < root->childlenORfilesize; i++){
        volatile struct fata *rootchild = (volatile struct fdata*)root->data[i];
    }
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
