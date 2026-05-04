#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>

#include "types.h"

void travelDir(char *name, file_t *files, dir_t *dirs, int *filesIDX, int *dirsIDX){
    DIR *dir = opendir(name);
    if(!dir){
        printf("Error reading dir %s\n", name);
        return;
    }
    struct dirent *entry;
    struct stat s;
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){ //? skip the .(current) and ..(prev) dirs
            continue;
        }
        char path[1024] = "";
        snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
        if(stat(path, &s) == 0 && S_ISDIR(s.st_mode)){
            dir_t newDIR;
            memset(&newDIR, 0, sizeof(newDIR));
            newDIR.alwaysFALSE = false;
            newDIR.children_amt = 0;
            newDIR.isroot = false;
            strcpy(newDIR.name, entry->d_name);
            dirs[*dirsIDX] = newDIR;
            (*dirsIDX)++;
            travelDir(path, files, dirs, filesIDX, dirsIDX);
        }else{
            file_t newfile;
            memset(&newfile, 0, sizeof(newfile));
            newfile.alwaysTRUE = true;
            newfile.datalen = 0;
            strcpy(newfile.name, entry->d_name);

            FILE *f = fopen(path, "rb");
            if(!f){
                printf("Unable to open file %s skipping its data\n", path);
                newfile.datalen = 0;
            }else{
                fseek(f, 0, SEEK_END);
                size_t size = ftell(f);
                rewind(f);
                uint32_t *filedata = malloc(size);
                fread(filedata, 1, size, f);
                fclose(f);
                newfile.datalen = size;
                newfile.data = filedata;
            }

            files[*filesIDX] = newfile;
            (*filesIDX)++;
            printf("%s\n", path);
        }
    }
    closedir(dir);
}

int main(){
    struct stat fsdir;
    if(!(stat("fs", &fsdir) == 0 && S_ISDIR(fsdir.st_mode))){
        int code = mkdir("fs", 0755);
        if(code != 0){
            printf("An error occured while creating dir\n");
            return 1;
        }else{
            printf("Directory created run again to generate binary\n");
            return 0;
        }
    }

    dir_t *dirs = (dir_t*)malloc(sizeof(dir_t)*1024);
    memset(dirs, 0, sizeof(dir_t)*1024);
    file_t *files = (file_t*)malloc(sizeof(file_t)*1024);
    memset(files, 0, sizeof(file_t)*1024);
    int *filesIDX = (int*)malloc(sizeof(int));
    int *dirsIDX = (int*)malloc(sizeof(int));
    *filesIDX = 0;
    *dirsIDX = 0;

    travelDir("fs", files, dirs, filesIDX, dirsIDX);

    char *final = (char*)malloc((sizeof(dir_t)*1024)+(sizeof(file_t)*1024));

    memset(final, 0, (sizeof(dir_t)*1024)+(sizeof(file_t)*1024));

    uint8_t *filedata = (uint8_t*)malloc(1024);
    size_t filedataSize = sizeof(uint8_t)*1024;
    int accum = 0;
    for(int i = 0; i<(*filesIDX); i++){
        file_t currfile = files[i];
        if(currfile.datalen > 0){
            if(currfile.datalen >= filedataSize){
                int inc = 1024;
                while (inc<currfile.datalen) {
                    inc += 1024;
                }
                uint8_t *newbuffer = (uint8_t*)malloc(filedataSize+inc);
                memcpy(newbuffer, filedata, filedataSize);
                filedataSize += inc;
                free(filedata);
                filedata = newbuffer;
            }
            memcpy(filedata+accum, currfile.data, currfile.datalen);
            int skipsize = sizeof(dir_t)*(*dirsIDX)+sizeof(file_t)*(*filesIDX);
            files[i].dataoffset = skipsize+accum;
            accum += currfile.datalen;
        }
    }

    memcpy(final, dirs, sizeof(dir_t)*(*dirsIDX));
    memcpy(final+sizeof(dir_t)*(*dirsIDX), files, sizeof(file_t)*(*filesIDX));
    memcpy(final+sizeof(dir_t)*(*dirsIDX)+sizeof(file_t)*(*filesIDX), filedata, filedataSize);

    FILE *output = fopen("output.bin", "wb");
    fwrite(final, (sizeof(dir_t)*(*dirsIDX))+(sizeof(file_t)*(*filesIDX))+accum, 1, output);
    fclose(output);

    free(dirs);
    free(files);
    free(final);
    free(filesIDX);
    free(dirsIDX);
    free(filedata);

    return 0;
}
