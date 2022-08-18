#pragma once
#include <cstdint>
#include "olc6502.h"
#include <array>

class Bus
{
public:
	Bus();
	~Bus();

public: //Devices auf dem Bus
	olc6502 cpu;

	// Fake RAM
	std::array<uint8_t, 64 * 1024> ram;

public: // BUS lesen und schreiben
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
};

