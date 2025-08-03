#include <stdint.h>

#define VBE_MODE_INFO_PHYS 0x900

void init_graphics_from_realmode_vbe();
void putpixel(int x, int y, uint32_t color);

// Only up to the fields we need; rest is ignored.
typedef struct __attribute__((packed)) {
    uint8_t  reserved1[0x10];    // skip to pitch / Xres
    uint16_t pitch;              // offset 0x10: bytes per scanline
    uint16_t xres;               // offset 0x12
    uint16_t yres;               // offset 0x14
    uint8_t  reserved2[0x19 - 0x16]; // pad until bpp
    uint8_t  bpp;                // offset 0x19
    uint8_t  reserved3[0x28 - 0x1A]; // pad until LFB addr
    uint32_t lfb_addr;           // offset 0x28: physical linear framebuffer
    // ... you can extend if needed
} vbe_mode_info_t;

typedef struct {
    uint32_t lfb_addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
} vbe_info_t;
