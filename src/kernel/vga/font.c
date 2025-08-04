#include "font.h"
#include "vga.h"
#include "font8x8_basic.h" // from https://github.com/dhepper/font8x8

// Helpers: adapt depending on what putpixel expects.
// If your putpixel wants a packed 16-bit RGB565 in the low bits:
static inline uint32_t rgb565(uint8_t r5, uint8_t g6, uint8_t b5) {
    return ((r5 & 0x1F) << 11) | ((g6 & 0x3F) << 5) | (b5 & 0x1F);
}

// If your putpixel wants 24-bit 0xRRGGBB:
static inline uint32_t rgb888(uint8_t r8, uint8_t g8, uint8_t b8) {
    return ((uint32_t)r8 << 16) | ((uint32_t)g8 << 8) | b8;
}

// Draw one character (8x8) at (x,y). No bounds checking.
void draw_char(char c, int x, int y, uint32_t color) {
    if ((unsigned char)c >= 128) c = '?'; // fallback for out-of-range
    const uint8_t *glyph = font8x8_basic[(unsigned char)c];
    for (int row = 0; row < 8; row++) {
        uint8_t row_byte = glyph[row];
        for (int col = 0; col < 8; col++) {
            if ((row_byte >> col) & 1) { // LSB is leftmost pixel per README
                putpixel(x + col, y + row, color);
            }
        }
    }
}

// Fixed-width string
void draw_string(const char *s, int x, int y, uint32_t color) {
    while (*s) {
        draw_char(*s, x, y, color);
        x += 8;
        s++;
    }
}

// Scaled-up string (nearest-neighbor scale)
void draw_string_scaled(const char *s, int x, int y, uint32_t color, int scale) {
    while (*s) {
        char c = *s;
        if ((unsigned char)c >= 128) c = '?';
        const uint8_t *glyph = font8x8_basic[(unsigned char)c];
        for (int row = 0; row < 8; row++) {
            uint8_t row_byte = glyph[row];
            for (int col = 0; col < 8; col++) {
                if ((row_byte >> col) & 1) {
                    int base_x = x + col * scale;
                    int base_y = y + row * scale;
                    for (int dy = 0; dy < scale; dy++) {
                        for (int dx = 0; dx < scale; dx++) {
                            putpixel(base_x + dx, base_y + dy, color);
                        }
                    }
                }
            }
        }
        x += 8 * scale;
        s++;
    }
}
