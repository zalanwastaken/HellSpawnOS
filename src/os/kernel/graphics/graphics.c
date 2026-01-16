#include"graphics.h"
#include "font8x8_basic.h"

VBE *graphicsInfo = (VBE*)0x0900; //? placed here by the bootloader

static inline uint32_t scale(uint8_t v, uint8_t bits) {
    return (v * ((1 << bits) - 1)) / 255;
}

uint32_t vbe_rgb(VBE* m, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t R = scale(r, m->red_mask)   << m->red_position;
    uint32_t G = scale(g, m->green_mask) << m->green_position;
    uint32_t B = scale(b, m->blue_mask)  << m->blue_position;
    return R | G | B;
}

void vbe_putpixel(VBE* m, int x, int y, uint32_t color) {
    uint8_t* fb = (uint8_t*)(uintptr_t)m->framebuffer;
    uint32_t bytes = m->bpp / 8;
    uint32_t off = y * m->pitch + x * bytes;

    for (uint32_t i = 0; i < bytes; i++) {
        fb[off + i] = (color >> (i * 8)) & 0xFF;
    }
}

// simple wrapper to draw a single char at (x, y)
void draw_char(int x, int y, char c, uint32_t color) {
    for(int row = 0; row < 8; row++) {
        uint8_t bits = font8x8_basic[(uint8_t)c][row];
        for(int col = 0; col < 8; col++) {
            if((bits >> col) & 1) { 
                vbe_putpixel(graphicsInfo, x + col, y + row, color);
            }
        }
    }
}

void vbe_draw_string(int x, int y, const char* str, uint32_t color) {
    int cursor_x = x;
    while(*str) {
        draw_char(cursor_x, y, *str, color);
        cursor_x += 8; // move to next char (8 pixels wide)
        str++;
    }
}

// draw a single scaled char at (x, y)
void draw_char_scaled(int x, int y, char c, uint32_t color, int scale) {
    for(int row = 0; row < 8; row++) {
        uint8_t bits = font8x8_basic[(uint8_t)c][row];
        for(int col = 0; col < 8; col++) {
            if((bits >> col) & 1) {
                // scale the pixel
                for(int dy = 0; dy < scale; dy++) {
                    for(int dx = 0; dx < scale; dx++) {
                        vbe_putpixel(graphicsInfo, x + col*scale + dx, y + row*scale + dy, color);
                    }
                }
            }
        }
    }
}

// draw a string horizontally with scaling
void vbe_draw_string_scaled(int x, int y, const char* str, uint32_t color, int scale) {
    int cursor_x = x;
    while(*str) {
        draw_char_scaled(cursor_x, y, *str, color, scale);
        cursor_x += 8 * scale; // move to next char
        str++;
    }
}
