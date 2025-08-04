#include "../fs/fs.h"

void render(){
    int file = findfile("root/tty");
    int buff[1024];
    readfile(file, 1024, buff);
}
