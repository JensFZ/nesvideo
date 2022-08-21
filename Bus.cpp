#include "Bus.h"


Bus::Bus()
{
	//Bus leeren
	for (auto &i : cpuRam) i = 0x00;

	//CPU mit dem bus verbinden
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{	// Erst die Catridge ansprechen, wenn diese nicht verantwortlich ist, kommt der cpu, ppu teil dran
	if (cart->cpuWrite(addr, data)) {
		
	} else if(addr >= 0x0000 && addr <= 0x1FFF) {
		cpuRam[addr & 0x07FF] = data;
	} else if (addr >= 0x2000 && addr <= 0x3FFF) {
		ppu.cpuWrite(addr & 0x0007, data);
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{	
	uint8_t data = 0x00;
	
	// Erst die Catridge ansprechen, wenn diese nicht verantwortlich ist, kommt der cpu, ppu teil dran
	if (cart->cpuRead(addr, data)) {

	} else 	if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpuRam[addr & 0x07FF];
	} else if (addr >= 0x2000 && addr <= 0x3FFF) {
		data = ppu.cpuRead(addr & 0x0007);
	}

	return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
	ppu.clock();

	if (nSystemClockCounter % 3 == 0) { // PPU Taktet 3 mal schneller als die CPU -> nur jeden dritten lauf
		cpu.clock();
	}

	nSystemClockCounter++;
}

