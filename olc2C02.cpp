#include "olc2C02.h"

uint8_t olc2C02::cpuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;

    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Adresse
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }

    return data;
}

void olc2C02::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t olc2C02::ppuRead(uint16_t addr, bool rdonly)
{
    return uint8_t();
}

void olc2C02::ppuWrite(uint16_t addr, uint8_t data)
{
}
