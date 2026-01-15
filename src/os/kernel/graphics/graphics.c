#include"graphics.h"

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
