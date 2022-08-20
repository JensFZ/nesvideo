#pragma once
#include <cstdint>
#include "Cartridge.h"
#include <memory>

#include "olcPixelGameEngine.h"

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

private:
	olc::Pixel palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256,240), olc::Sprite(256,240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128,128), olc::Sprite(128,128) };

public:
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	int16_t scanline = 0; 
	int16_t cycle = 0;

};

