#include"disk.h"
#include "../serial/serial.h"
#include"../idt/idt.h"

#define ATA_PRIMARY_DATA      0x1F0
#define ATA_PRIMARY_ERROR     0x1F1
#define ATA_PRIMARY_SECCOUNT  0x1F2
#define ATA_PRIMARY_LBA_LOW   0x1F3
#define ATA_PRIMARY_LBA_MID   0x1F4
#define ATA_PRIMARY_LBA_HIGH  0x1F5
#define ATA_PRIMARY_DRIVE     0x1F6
#define ATA_PRIMARY_STATUS    0x1F7
#define ATA_PRIMARY_COMMAND   0x1F7

#define ATA_CMD_READ_SECTORS  0x20
#define ATA_CMD_WRITE_SECTORS 0x30

static inline void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

// wait for BSY = 0 and DRQ = 1
static void ata_wait_busy(void) {
    while (inb(ATA_PRIMARY_STATUS) & 0x80); // BSY
    while (!(inb(ATA_PRIMARY_STATUS) & 0x08)); // DRQ
}

void ata_read_sector(uint32_t lba, uint8_t* buffer) {
    asm volatile("cli");

    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, 1); // read 1 sector
    outb(ATA_PRIMARY_LBA_LOW,  lba & 0xFF);
    outb(ATA_PRIMARY_LBA_MID,  (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMMAND, ATA_CMD_READ_SECTORS);

    ata_wait_busy();

    for (int i = 0; i < 256; i++) { // 512 bytes / 2 = 256 words
        uint16_t data = inw(ATA_PRIMARY_DATA);
        buffer[i*2] = data & 0xFF;
        buffer[i*2+1] = (data >> 8) & 0xFF;
    }

    asm volatile("sti");
}

void ata_write_sector(uint32_t lba, uint8_t* buffer) {
    asm volatile("cli");

    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, 1); // write 1 sector
    outb(ATA_PRIMARY_LBA_LOW,  lba & 0xFF);
    outb(ATA_PRIMARY_LBA_MID,  (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMMAND, ATA_CMD_WRITE_SECTORS);

    ata_wait_busy();

    for (int i = 0; i < 256; i++) {
        uint16_t data = buffer[i*2] | (buffer[i*2+1] << 8);
        outw(ATA_PRIMARY_DATA, data);
    }

    // send cache flush
    outb(ATA_PRIMARY_COMMAND, 0xE7); 
    ata_wait_busy();

    asm volatile("sti");
}
