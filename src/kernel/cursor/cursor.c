#include "../io/io.h"  // for outb

void move_cursor(int pos) {
    outb(0x3D4, 0x0F);              // Tell VGA we’re setting low cursor byte
    outb(0x3D5, pos & 0xFF);        // Send low byte
    outb(0x3D4, 0x0E);              // Now high byte
    outb(0x3D5, (pos >> 8) & 0xFF); // Send high byte
}
