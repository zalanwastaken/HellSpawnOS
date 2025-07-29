#define FSROOT 0x0000
#include<stdint.h>
#pragma once
int chkfileID(int id);
void newfile(int id, const char name[], int dataloc, int size, int location);
void addchild(int parent_location, int child_location);
void initfsroot();
int findfile(const char *path);

struct fdata{
    int isdir;
    int id;
    int namelen;
    int childlenORfilesize;
    int ptrtodata;
    int data[];
};
