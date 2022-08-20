#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Cartridge
{
public: 
	Cartridge(const std::string& sFilename);
	~Cartridge();


private:
	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	uint8_t nMapperID = 0; // Welcher Mapper wird verwendet
	uint8_t nPRGBanks = 0; // Anzahl der Programm Bänke
	uint8_t nCHRBanks = 0; // Anzahl der CharacterRom Bänke

public:
	bool cpuRead(uint16_t addr, bool rdonly = false);
	bool cpuWrite(uint16_t addr, uint8_t data);

	bool ppuRead(uint16_t addr, bool rdonly = false);
	bool ppuWrite(uint16_t addr, uint8_t data);

};

