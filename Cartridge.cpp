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

        if (header.mapper1 & 0x04) {
            ifs.seekg(512, std::ios_base::cur); //512 Byte überspringen
        }

        // Header ID lesen
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

        // Dateityp ermitteln
        uint8_t nFileType = 1;

        switch (nFileType) {
            case 0:
                // Aktuell nicht unterstützt
                break;
            case 1:
                nPRGBanks = header.prg_rom_chunks;
                vPRGMemory.resize(nPRGBanks * 16384);
                ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

                nCHRBanks = header.chr_rom_chunks;
                vCHRMemory.resize(nCHRBanks * 8192);
                ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());

                break;
            case 2:
                // Aktuell nicht unterstützt
                break;
        }
        ifs.close();

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
