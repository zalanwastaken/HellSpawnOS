#include "fs.h"
#include "../io/serial.h"
#include "../utils.h"

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

int readfile(int nodeptr, int bytestoread, int buff[]){
    volatile struct fdata *node = (volatile struct fdata*)nodeptr;
    if(node->isdir == 1){
        return -1;
    }
    if(node->childlenORfilesize < bytestoread){
        return -1;
    }
    int *data = (int*)node->ptrtodata;
    for(int i = 0; i < bytestoread; i++){
        buff[i] = data[i];
    }
    return 0;
}

int writefile(int nodeptr, int bytestowrite, int buff[]){
    volatile struct fdata *node = (volatile struct fdata*)nodeptr;
    if(node->isdir == 1){
        return -1;
    }
    if(node->childlenORfilesize < bytestowrite){
        //! impl handeling later
        return -1;
    }
    int *data = (int*)node->ptrtodata;
    for(int i = 0; i < bytestowrite; i++){
        data[i] = buff[i];
    }
    return 0;
}

int getNodeSize(int ptr){
    volatile struct fdata *node = (volatile struct fdata*)ptr;
    if(node->isdir == 1){
        return(sizeof(int)*5+(sizeof(int)*(node->childlenORfilesize+node->namelen)));
    }else{
        return(sizeof(int)*5+(sizeof(int)*(node->namelen)));
    }
}

void clear_low_memory() {
    unsigned char* ptr = (unsigned char*)0x0000;
    for (int i = 0; i < 0x500; i++) {
        ptr[i] = 0x00;
    }
}

void initfs(){
    clear_low_memory();

    int NOchildren[1] = {};
    newdir(0xBEEF, "root", NOchildren, 0, FSROOT);
    newfile(rand(), "kernel", 0x1000, (*(int*)0x7E0F)*512, 0x00FF);
    addchild(FSROOT, 0x00FF);

    unsigned int loc = FSROOT+getNodeSize(FSROOT)+1;
    newfile(rand(), "kbd", 0x7E00, 2, loc);
    addchild(FSROOT, loc);
    serial_write_hex(loc);
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
