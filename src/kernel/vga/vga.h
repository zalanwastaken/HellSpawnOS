#include <stdint.h>

#define VBE_MODE_INFO_PHYS 0x900

void init_graphics_from_realmode_vbe();
void putpixel(int x, int y, uint32_t color);

// Only up to the fields we need; rest is ignored.
typedef struct __attribute__((packed)) {
    uint8_t  reserved1[0x10];               // 0x00 - 0x0F
    uint16_t pitch;                         // 0x10: bytes per scanline
    uint16_t xres;                          // 0x12: horizontal resolution
    uint16_t yres;                          // 0x14: vertical resolution
    uint8_t  reserved2[0x19 - 0x16];        // padding: 0x16-0x18
    uint8_t  bpp;                           // 0x19: bits per pixel
    uint8_t  reserved3[0x28 - 0x1A];        // padding: 0x1A-0x27
    uint32_t lfb_addr;                      // 0x28: physical linear framebuffer address
    // ... can extend with more fields if needed
} vbe_mode_info_t;

typedef struct {
    uint32_t lfb_addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
} vbe_info_t;
