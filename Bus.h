#pragma once
#include <cstdint>
#include "olc6502.h"
#include "olc2C02.h"
#include "Cartridge.h"
#include <array>
#include <memory>

class Bus
{
public:
	Bus();
	~Bus();

public: //Devices auf dem Bus
	olc6502 cpu; // Prozessor
	olc2C02 ppu; // Pixel Processing Unit

	// Fake RAM
	std::array<uint8_t, 2 * 1024> cpuRam; // 2Kb Ram

	std::shared_ptr<Cartridge> cart;

public: // BUS lesen und schreiben
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public:
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	//Taktz?hler
	uint32_t nSystemClockCounter = 0;
};

