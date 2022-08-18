#pragma once
#include <cstdint>
class olc2C02
{
public:
	olc2C02();
	~olc2C02();


private:
	// Speicher:
	uint8_t tblName[2][1024];		// 2KB sind eigentlich 2x1KB
	uint8_t	tblPalette[32];			// Paletten
	uint8_t	tblPattern[2][4096];	// wird eigentlich nicht benötigt, da es in der Cartridge eigentlich sitzt

public:
	// BUS ansprechen
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void	cpuWrite(uint16_t addr, uint8_t data);

	// PPU ansprechen
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void	ppuWrite(uint16_t addr, uint8_t data);


private:
	std::shared_ptr<Cartridge> cart;

public:
	//Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
};

