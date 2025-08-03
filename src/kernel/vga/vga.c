#include "vga.h"

volatile uint32_t *framebuffer = 0;
uint32_t screen_pitch = 0;
uint32_t screen_width = 0;
uint32_t screen_height = 0;

void init_graphics_from_realmode_vbe() {
    vbe_mode_info_t *modeinfo = (vbe_mode_info_t *)(uintptr_t)VBE_MODE_INFO_PHYS;

    // Basic sanity: check that bpp is reasonable
    if (modeinfo->bpp < 16) {
        // fallback or error
        return;
    }

    vbe_info_t info;
    info.lfb_addr = modeinfo->lfb_addr;
    info.pitch    = modeinfo->pitch;
    info.width    = modeinfo->xres;
    info.height   = modeinfo->yres;
    info.bpp      = modeinfo->bpp;

    // For now assume identity paging so physical == virtual
    framebuffer = (volatile uint32_t *)(uintptr_t)info.lfb_addr;
    screen_pitch  = info.pitch;
    screen_width  = info.width;
    screen_height = info.height;
}
void putpixel(int x, int y, uint32_t color) {
    if ((uint32_t)x >= screen_width || (uint32_t)y >= screen_height) return;
    uint8_t *row = (uint8_t *)framebuffer + y * screen_pitch;
    uint32_t *pixel = (uint32_t *)(row + x * 4);
    *pixel = color;
}
