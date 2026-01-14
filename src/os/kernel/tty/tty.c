#include "../fs/fs.h"
#include "../vga/vga.h"
#include "../io/serial.h"
#include "../utils.h"

void flush(char str[], int x, int y){
    draw_string(str, x, y, CLR_WHITE);
}

int renderTTY(){
    int file = getfilePTR("root/tty");
    if(file == -1){
        return -1;
    }else{
        volatile struct fdata* fileF = (volatile struct fdata*)file;
        serial_write_hex(fileF->ptrtodata);
    }

    int x = 0; // only used for cursor placement
    int y = 0;
    int buff[1024];
    readfile(file, 1024, buff);
    char auxbuff[1024];
    int auxbuffpoint = 0;
    for(int i = 0; i < 1024; i++){
        char c = (char)buff[i];
        if(c == '\0') break; // stop at end of string
        if(c == '\n'){
            auxbuff[auxbuffpoint] = '\0';
            flush(auxbuff, 0, y);
            auxbuffpoint = 0;
            y += 8; // move to next line
            x = 0;
            continue;
        }
        auxbuff[auxbuffpoint++] = c;
        x += 8;
    }
    // Flush last line if not empty
    if(auxbuffpoint > 0){
        auxbuff[auxbuffpoint] = '\0';
        flush(auxbuff, 0, y);
    }
    drawrect(x, y+8, x+8, y+8, 0xFFFFFF);
    return 0;
}
