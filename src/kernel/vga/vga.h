#include <stdint.h>

#define VBE_MODE_INFO_PHYS 0x900
//* colors
#define CLR_RED       0xFF0000
#define CLR_GREEN     0x00FF00
#define CLR_BLUE      0x0000FF
#define CLR_BLACK     0x000000
#define CLR_WHITE     0xFFFFFF
#define CLR_YELLOW    0xFFFF00
#define CLR_CYAN      0x00FFFF
#define CLR_MAGENTA   0xFF00FF
#define CLR_GRAY      0x808080
#define CLR_DARKGRAY  0x404040
#define CLR_LIGHTGRAY 0xC0C0C0
#define CLR_ORANGE    0xFFA500
#define CLR_BROWN     0x8B4513
#define CLR_PURPLE    0x800080
#define CLR_PINK      0xFFC0CB
#define CLR_LIME      0xBFFF00
#define CLR_TEAL      0x008080
#define CLR_NAVY      0x000080
#define CLR_MAROON    0x800000
#define CLR_OLIVE     0x808000
#define CLR_GOLD      0xFFD700
#define CLR_SKYBLUE   0x87CEEB
#define CLR_VIOLET    0xEE82EE

void init_graphics_from_realmode_vbe();
void putpixel(int x, int y, uint32_t color);
void draw_char(char c, int x, int y, uint32_t color);
void draw_string(const char *s, int x, int y, uint32_t color);
void draw_string_scaled(const char *s, int x, int y, uint32_t color, int scale);
void clearscreen(int color);
void drawrect(int sx, int sy, int ex, int ey, int color);
void drawrectFill(int sx, int sy, int ex, int ey, int color);

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
