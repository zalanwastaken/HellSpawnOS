#include "vga.h"

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
