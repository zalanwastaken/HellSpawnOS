#include "vga.h"
#include "font8x8_basic.h" // from https://github.com/dhepper/font8x8

volatile int *framebuffer = 0;
uint32_t screen_pitch = 0;
uint32_t screen_width = 0;
uint32_t screen_height = 0;

void init_graphics_from_realmode_vbe() {
    vbe_mode_info_t *modeinfo = (vbe_mode_info_t *)(uintptr_t)VBE_MODE_INFO_PHYS;

    // Basic sanity: check that bpp is reasonable
    if (modeinfo->bpp < 16) {
        // fallback or error
        //return;
    }

    vbe_info_t info;
    info.lfb_addr = modeinfo->lfb_addr;
    info.pitch    = modeinfo->pitch;
    info.width    = modeinfo->xres;
    info.height   = modeinfo->yres;
    info.bpp      = modeinfo->bpp;

    // For now assume identity paging so physical == virtual
    framebuffer = (int*)info.lfb_addr;
    screen_pitch  = info.pitch;
    screen_width  = info.width;
    screen_height = info.height;
}
void putpixel(int x, int y, uint32_t color) {
    if ((uint32_t)x >= screen_width || (uint32_t)y >= screen_height) return;
    uint8_t *row = (uint8_t *)framebuffer + y * screen_pitch;
    // Convert 0xRRGGBB to 16-bit 5:6:5
    uint16_t r = (color >> 19) & 0x1F;
    uint16_t g = (color >> 10) & 0x3F;
    uint16_t b = (color >> 3)  & 0x1F;
    uint16_t pixel16 = (r << 11) | (g << 5) | b;
    ((uint16_t*)row)[x] = pixel16;
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

void clearscreen(int color){
    for(int i = 0; i < 600; i++){
        for(int f = 0; f < 800; f++){
            putpixel(f, i, color); //! RR GG BB
        }
    }
}

void drawrect(int sx, int sy, int ex, int ey, int color){
    for(int i = sx; i < ex; i++){
        putpixel(i, sy, color);
        putpixel(i, ey, color);
    }
    for(int i = sy; i < ey; i++){
        putpixel(sx, i, color);
        putpixel(ex, i, color);
    }
}

void drawrectFill(int sx, int sy, int ex, int ey, int color){
    for(int i = sx; i < ex; i++){
        putpixel(i, sy, color);
        putpixel(i, ey, color);
    }
    for(int i = sy; i < ey; i++){
        putpixel(sx, i, color);
        putpixel(ex, i, color);
    }
    for(int i = sy; i < ey; i++){
        for(int f = sx; f < ex; f++){
            putpixel(f, i, color); //! RR GG BB
        }
    }
}
