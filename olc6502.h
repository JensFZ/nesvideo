#pragma once
//#include "Bus.h"
#include <cstdint>

class Bus;

class olc6502
{
public:
	olc6502();
	~olc6502();

public:
	enum FLAGS6502 { // Bit: NVUBDIZC
		C = (1<<0), // Carry Bit
		Z = (1<<1), // Zero
		I = (1<<2), // Disable Interupts
		D = (1<<3), // Decimal Mode (wird nicht verwendet vom NES)
		B = (1<<4), // Break
		U = (1<<5), // Unused
		V = (1<<6), // Overflow
		N = (1<<7), // Negative
	};

	//register:
	uint8_t a = 0x00;		//Accumulator
	uint8_t x = 0x00;		// X Register
	uint8_t y = 0x00;		// y Register
	uint8_t stkp = 0x00;	// Stack Pointer
	uint16_t pc = 0x0000;	// Program Counter
	uint8_t status = 0x00;	// Status

	void ConnectBus(Bus* n) {
		bus = n;
	}

private:
	Bus* bus = nullptr;
	uint8_t read(uint16_t a); // Adresse lesen
	void write(uint16_t a, uint8_t d); // Adresse schreiben

	uint8_t getFlag(FLAGS6502 f); // Flag lesen
	void	setFlag(FLAGS6502 f, bool v); // Flag setzen
};

