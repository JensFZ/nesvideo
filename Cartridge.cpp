#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge(const std::string& sFilename)
{
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs;
    ifs.open(sFilename, std::ifstream::binary);
    if (ifs.is_open()) {
        // Header lesen
        ifs.read((char*)&header, sizeof(sHeader));
    }

}

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
