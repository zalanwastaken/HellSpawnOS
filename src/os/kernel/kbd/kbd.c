#include "../io/io.h"
//#include "../io/serial.h"
#include "../utils.h"
#include <stdint.h>

const char scancode_map[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char scancode_map_shift[] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int shift_down = 0;
static int capslock_on = 0;

void irq1_handler() {
    uint8_t scancode = inb(0x60);

    // Shift press/release
    if (scancode == 0x2A || scancode == 0x36) { // Shift pressed
        shift_down = 1;
        EOI(1);
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) { // Shift released
        shift_down = 0;
        EOI(1);
        return;
    }

    // Caps Lock toggle (only on key press, not release)
    if (scancode == 0x3A) {
        capslock_on = !capslock_on;
        EOI(1);
        return;
    }

    if (scancode < sizeof(scancode_map)) {
        char c;
        if (shift_down) {
            c = scancode_map_shift[scancode];
        } else {
            c = scancode_map[scancode];
            // apply capslock only for letters when shift is NOT down
            if (c >= 'a' && c <= 'z' && capslock_on) {
                c = c - 'a' + 'A';
            }
        }

        if (c) {
            ((volatile char*)0x7E00)[0] = c;
        }
    }


    EOI(1); // 1 for kbd
}
