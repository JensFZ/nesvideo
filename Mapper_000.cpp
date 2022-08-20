#include "Mapper_000.h"

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);

        return true;
    }
    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFFF) {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    // Dieser Mapper nutzt kein schreiben hier
    //if (addr >= 0x0000 && addr <= 0x1FFFF) {

    //    return true;
    //}

    return false;
}
