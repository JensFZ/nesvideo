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


	// Bus verbinden
	void ConnectBus(Bus* n) {
		bus = n;
	}

	// Addressing Modes
	uint8_t IMP(); uint8_t IMM();
	uint8_t ZP0(); uint8_t ZPX();
	uint8_t ZPY(); uint8_t REL();
	uint8_t ABS(); uint8_t ABX();
	uint8_t ABY(); uint8_t IND();
	uint8_t IZX(); uint8_t IZY();

private:
	Bus* bus = nullptr;
	uint8_t read(uint16_t a); // Adresse lesen
	void write(uint16_t a, uint8_t d); // Adresse schreiben

	uint8_t getFlag(FLAGS6502 f); // Flag lesen
	void	setFlag(FLAGS6502 f, bool v); // Flag setzen
};

