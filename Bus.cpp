#include "Bus.h"

Bus::Bus()
{
	//Bus leeren
	for (auto &i : ram) i = 0x00;

	//CPU mit dem bus verbinden
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if(addr >= 0x0000 && addr <= 0xFFFF) {
		ram[addr] = data;
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		return ram[addr];
	}

	return 0x00;
}

