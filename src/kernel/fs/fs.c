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

// Parse the nth directory name from path into buff
void parsepath(const char path[], char buff[], unsigned int dirN) {
    int i = 0, count = 0, j = 0;

    while (path[i] == '/') i++; // skip leading slash

    while (path[i] != '\0') {
        if (count == dirN) {
            while (path[i] != '/' && path[i] != '\0') {
                buff[j++] = path[i++];
            }
            break;
        }

        while (path[i] != '/' && path[i] != '\0') i++;
        while (path[i] == '/') i++;
        count++;
    }

    buff[j] = '\0';

    if (count < dirN) {
        buff[0] = '\0';
    }
}

int matchname(char name[], char nodename[], int n){
    for(int i = 0; i < n; i++){
        if((int)name[i] != (int)nodename[i]){
            return 0;
        }
    }
    return 1;
}

int findFileRecur(char name[], char path[], int iter, int nodeptr){
    volatile struct fdata* node = (volatile struct fdata*)nodeptr;
    char nodename[32];
    for(int i = 0; i < node->namelen; i++){
        nodename[i] = node->data[i];
    }
    if(strncmp(name, nodename, node->namelen) == 0){
        return nodeptr;
    }
    //check if is a dir
    if(node->isdir == 1){
        for(int i = 0; i < node->childlenORfilesize; i++){
            char buff[32];
            parsepath(path, buff, iter+1);
            if(strncmp(buff, "", 1) == 0){
                return -1;
            }
            int ret = findFileRecur(buff, path, iter+1, node->data[node->namelen+i+1]);
            if(ret != -1){
                return ret;
            }
        }
    }
    return -1;
}

int findfile(const char *path){
    char buff[32];
    parsepath(path, buff, 0);
    return findFileRecur(buff, path, 0, FSROOT);
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
