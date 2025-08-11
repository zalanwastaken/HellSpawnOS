#include "../utils.h"
#include "../io/serial.h"
#include "../vga/vga.h"
#include "../io/io.h"

void draw_cursor(int x, int y) {
    // Just a tiny cross
    putpixel(x, y, 0xFFFFFF);
    putpixel(x+1, y, 0xFFFFFF);
    putpixel(x-1, y, 0xFFFFFF);
    putpixel(x, y+1, 0xFFFFFF);
    putpixel(x, y-1, 0xFFFFFF);
}

int mouse_x;
int mouse_y;
static uint8_t mouse_cycle = 0;
static uint8_t mouse_packet[3];

void irq44_handler() {
    uint8_t byte = inb(0x60);

    mouse_packet[mouse_cycle++] = byte;

    if (mouse_cycle == 3) {
        mouse_cycle = 0;

        // Parse packet
        int8_t dx = (int8_t)mouse_packet[1];
        int8_t dy = (int8_t)mouse_packet[2];

        mouse_x += dx;
        mouse_y -= dy;

        // Clamp
        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x >= SCREEN_WIDTH) mouse_x = SCREEN_WIDTH - 1;
        if (mouse_y >= SCREEN_HEIGHT) mouse_y = SCREEN_HEIGHT - 1;

        // Draw your spicy cursor
        draw_cursor(mouse_x, mouse_y);
    }

    EOI(12);
}


void ps2_mouse_init() {
    // Tell the PS/2 controller to enable the auxiliary device (mouse)
    outb(0x64, 0xA8); // Enable mouse port

    // Enable interrupts for mouse
    outb(0x64, 0x20);            // Read command byte
    uint8_t status = inb(0x60);
    status |= 3;                // Enable IRQ12
    outb(0x64, 0x60);           // Write command byte
    outb(0x60, status);

    // Send magic enable sequence to mouse
    outb(0x64, 0xD4);           // Tell controller: next byte is for mouse
    outb(0x60, 0xF4);           // Enable packet streaming

    // Read ACK (should be 0xFA)
    if (inb(0x60) != 0xFA) {
        // Mouse didn't acknowledge, scream in binary
        panic(0x0001);
    }
}
