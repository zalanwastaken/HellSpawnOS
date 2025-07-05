#include "../io/io.h"
#include "../io/serial.h"
#include "../utils.h"
#include <stdint.h>

const char scancode_map[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void irq1_handler() {
    uint8_t scancode = inb(0x60);

    if (scancode < sizeof(scancode_map)) {
        char c = scancode_map[scancode];
        if (c) {
            char out[] = {c, '\0'};
            for (int i = 0; out[i] != '\0'; i++) {
                ((volatile char*)0x7E00)[i] = out[i];
            }
        }
    }

    EOI(1); // 1 for kbd
}
