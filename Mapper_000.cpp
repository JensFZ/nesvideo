#include "Mapper_000.h"

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
}


Mapper_000::~Mapper_000()
{
}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // PRGROM = 16KB
        //     CPU Address Bus          PRG ROM
        //     0x8000 -> 0xBFFF: Map    0x0000 -> 0x3FFF
        //     0xC000 -> 0xFFFF: Mirror 0x0000 -> 0x3FFF
        // PRGROM = 32KB
        //     CPU Address Bus          PRG ROM
        //     0x8000 -> 0xFFFF: Map    0x0000 -> 0x7FFF
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
