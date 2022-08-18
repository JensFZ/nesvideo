#include "Cartridge.h"

uint8_t Cartridge::cpuRead(uint16_t addr, bool rdonly)
{
    return uint8_t();
}

void Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t Cartridge::ppuRead(uint16_t addr, bool rdonly)
{
    return uint8_t();
}

void Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
}
