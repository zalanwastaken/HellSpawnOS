#pragma once
#include <stdint.h>

void ata_read_sector(uint32_t lba, uint8_t* buffer);
void ata_write_sector(uint32_t lba, uint8_t* buffer);
void ata_read_sector_safe(uint32_t lba, uint8_t *buffer); //? this disables intrrupts before reading
void ata_write_sector_safe(uint32_t lba, uint8_t *buffer); //? this disables intrrupts before writing
