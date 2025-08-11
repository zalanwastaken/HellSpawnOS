#define FSROOT 0x0000
#pragma once

void initfs();
void newfile(int id, const char name[], int dataloc, int size, int location);
void addchild(int parent_location, int child_location);
int findfile(const char *path);
int readfile(int nodeptr, int bytestoread, int buff[]);
int writefile(int nodeptr, int bytestowrite, int buff[]);
int getNodeSize(int ptr);

struct fdata{
    int isdir;
    int id;
    int namelen;
    int childlenORfilesize;
    int ptrtodata;
    int data[];
};
