#include "fs.h"
#include "../io/serial.h"
#include "../utils.h"

void initfsroot(){
    //* root dir
    volatile struct fdata *root = (volatile struct fdata*)FSROOT;
    root->isdir = 1; // this is a dir(root)
    root->id = 0xDEAD;
    root->namelen = 3; // starting from 0
    for(int i = 0; i <= root->namelen; i++){
        char name[4] = "root";
        root->data[i] = name[i]; // set its name
    }
    root->childlenORfilesize = 0;
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
void getLastPathPart(const char *path, char *buff) {
    char temp[32];
    int i = 0;
    while (1) {
        parsepath(path, temp, i);
        if (temp[0] == '\0') {
            break;
        }

        // Manual strcpy: copy temp into buff
        int j = 0;
        while (temp[j] != '\0') {
            buff[j] = temp[j];
            j++;
        }
        buff[j] = '\0'; // Null-terminate
        i++;
    }
}

int findFileRecur(char name[], const char path[], int iter, int nodeptr){
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
            if((int)buff[0] == (int)'\0'){
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
    getLastPathPart(path, buff);
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

void newdir(int id, const char name[], int children[], int childrenAMT, int location){
    volatile struct fdata *dir = (volatile struct fdata*)location;
    dir->isdir = 1;
    dir->id = id;
    dir->namelen = strlen(name);
    dir->childlenORfilesize = childrenAMT;
    for(int i = 0; i <= dir->namelen; i++){
        dir->data[i] = name[i];
    }
    for(int i = 0; i <= dir->childlenORfilesize; i++){
        dir->data[dir->namelen+i+1] = children[i];
    }
}

void addchild(int parent_location, int child_location){
    volatile struct fdata *parent = (volatile struct fdata*)parent_location;
    if(parent->isdir == 1){
        parent->childlenORfilesize++;
        parent->data[parent->namelen+parent->childlenORfilesize] = child_location;
    }
}
