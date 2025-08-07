#include "utils.h"
#include "io/io.h"
#include "io/serial.h"

void EOI(int irq_num){
    if (irq_num >= 8) outb(0xA0, 0x20);  // slave PIC
    outb(0x20, 0x20);                    // master PIC
}

void int_to_intstr(int num, char* buffer) {
    int i = 0;
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    if (num < 0) {
        buffer[i++] = '-';
        num = -num;
    }

    // Store digits in reverse
    int start = i;
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    buffer[i] = '\0';

    // Reverse the digits
    for (int j = start, k = i - 1; j < k; j++, k--) {
        char tmp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = tmp;
    }
}

//! also adds null terminator
void str_to_int(char charbuff[], int intbuff[]){
    for(int i = 0; i < strlen(charbuff)+1; i++){
        intbuff[i] = (int)charbuff[i];
    }
}

int strncmp(const char* s1, const char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

uint32_t hex_to_int(const char* str) {
    uint32_t result = 0;
    int i = (str[0] == '0' && str[1] == 'x') ? 2 : 0;

    while (str[i] != '\0') {
        char c = str[i];
        result <<= 4;

        if (c >= '0' && c <= '9')
            result |= c - '0';
        else if (c >= 'A' && c <= 'F')
            result |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            result |= c - 'a' + 10;
        else
            break; // Invalid character

        i++;
    }

    return result;
}

void int_to_hex(uint32_t value, char* buffer) {
    const char* hex_chars = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int i = 0; i < 8; i++) {
        buffer[9 - i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    buffer[10] = '\0';
}

void concat(char *dest, const char *src) {
    // Move to the end of dest
    while (*dest) dest++;

    // Copy src to the end of dest
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }

    // Null terminate the result
    *dest = '\0';
}

void serial_write_hex(int p){
    char buff[12];
    int_to_hex(p, buff);
    concat(buff, "\n");
    serial_write(buff);
}

unsigned int strlen(const char str[]) {
    unsigned int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

static unsigned int seed = 1;
unsigned int rand(){
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}

void srand(unsigned int s){
    seed = s;
}
