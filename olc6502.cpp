#include "olc6502.h"
#include "Bus.h"

olc6502::olc6502() 
{
	using a = olc6502; // nur da um die nachfolgende Tabelle etwas kürzer zu gestallten, ansonsten müsste anstelle von &a:: &olc6502:: jedesmal stehen

	// Abbildung der Grafik auf Seite 22 der Dokumentation des 6502 CPUs (zu finden im Ordner Docs\CPU)
	lookup = { 
		// {name, opcode, Adressingmode, Taktzyclen}
		// Dadurch wird jeder opcode einen hexwert zugewiesen. BRK = 0x00, ORA = 0x01, ...
		// Dies ermöglicht, dass man den Hexwert aus dem Ram direkt als index des Arrays verwendet
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

olc6502::~olc6502()
{
}

uint8_t olc6502::read(uint16_t a)
{
	return bus->read(a, false);
}

void olc6502::write(uint16_t a, uint8_t d)
{
	bus->write(a, d);
}

// gibt den wert des angegebenen flags des statusregisters zurück
uint8_t olc6502::getFlag(FLAGS6502 f)
{
	return ((status & f) > 0 ? 1: 0);
}

// Setzt oder leert das angegebene bit des status registers
void olc6502::setFlag(FLAGS6502 f, bool v)
{
	if (v) {
		status |= f;
	}
	else {
		status &= ~f;
	}

}

void olc6502::clock()
{
	if (cycles == 0) { // nur wenn cycles auf 0 steht neuen Opcode laden. Ansonsten ist der vorhergehende opcode noch nicht komplett verarbeitet
		opcode = read(pc); // Opcode lesen aus dem Bus
		pc++; // Program counter +1

		// Opcode Takte auslesen
		cycles = lookup[opcode].cycles;

		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)(); // Adressmode funktion aufrufen
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)(); // Oppcode funktion aufrufen

		cycles += (additional_cycle1 & additional_cycle2); // zusätzliche takte auf die Cycles draufrechnen (im tutorial ist hier ein &, muss ggf. nochmal gedebugt werden)
	}

	cycles--;
}

void olc6502::reset()
{
	a = 0;
	x = 0;
	y = 0;
	stkp = 0;
	status = 0x00 | U;
	
	// PC kann vom Rom gesetzt werden und der Entry Point liegt an adresse 0xFFFC + 1
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	pc = (hi << 8) | lo;

	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	cycles = 8;
}

void olc6502::irq()
{
	if (getFlag(I) == 0) {
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;

		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 7;
	}
}

void olc6502::nmi()
{
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;

	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;

}


#pragma region Addressing Modes

uint8_t olc6502::IMP() // Implied -> keine Daten angegeben aber A register könnte benötigt werden
{
	fetched = a;
	return 0;
}

uint8_t olc6502::IMM() // Immediate mode -> Daten sind teil der instruction (1 Byte) -> aus dem BUS lesen
{
	addr_abs = pc++;
	return 0;
}

uint8_t olc6502::ZP0() // Zero Page Adressing
{
	// Zeropage = 0x0000 - 0x00FF -> das Byte gibt den Adressraum in Page 0x00 an.
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPX() // X Register Zero Page Adressing
{
	addr_abs = (read(pc) + x); // Adresse aus Bus + X Positionen aus dem X Register
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPY() // Y Register Zero Page Adressing
{
	addr_abs = (read(pc) + y); // Adresse aus Bus + X Positionen aus dem Y Register
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::REL() // Relative Mode. Wird für Sprünge verwendet. das ziel kann nur -128 und +127 positionen von der Adresse entfernt sein. 
{
	addr_rel = read(pc);
	pc++;
	if (addr_rel & 0x80)
	{
		addr_rel |= 0xFF00;
	}
	return 0;
}

uint8_t olc6502::ABS() { // Absolute Adresse angegeben
	uint16_t lo = read(pc); // Lower bit adress lesen, z.B. 0x02
	pc++;
	uint16_t hi = read(pc); //Higher bit adress lesen z.B. 0xA2
	pc++;

	addr_abs = (hi << 8) | lo; // Higher und Lower adress zusammensetzeb zu z.B. 0xA202

	return 0;
}

uint8_t olc6502::ABX() { // Absolute Adresse angegeben + X Register
	uint16_t lo = read(pc); // Lower bit adress lesen, z.B. 0x02
	pc++;
	uint16_t hi = read(pc); //Higher bit adress lesen z.B. 0xA2
	pc++;

	addr_abs = (hi << 8) | lo; // Higher und Lower adress zusammensetzeb zu z.B. 0xA202
	addr_abs += x; // X Register Schritte weiter gehen

	// Taktzyklen ggf. erweitern
	if((addr_abs & 0xFF00) != (hi << 8)) { // Wenn die Adresse größer als eine angegebene Page ist -> +1 Takt
		return 1;
	}
	return 0;
}

uint8_t olc6502::ABY() { // Absolute Adresse angegeben + X Register
	uint16_t lo = read(pc); // Lower bit adress lesen, z.B. 0x02
	pc++;
	uint16_t hi = read(pc); //Higher bit adress lesen z.B. 0xA2
	pc++;

	addr_abs = (hi << 8) | lo; // Higher und Lower adress zusammensetzeb zu z.B. 0xA202
	addr_abs += y; // Y Register Schritte weiter gehen

	// Taktzyklen ggf. erweitern
	if ((addr_abs & 0xFF00) != (hi << 8)) { // Wenn die Adresse größer als eine angegebene Page ist -> +1 Takt
		return 1;
	}
	return 0;
}

uint8_t olc6502::IND() //Indirekte Adressierung
{
	uint16_t ptr_lo = read(pc); // Pointer low Adress lesen
	pc++;
	uint16_t ptr_hi = read(pc); // Pointer high adress lesen
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo; // Pointer zusammensetzen

	// Da die 6502 Hardware einen Bug hat, wenn der low pointer auf 0x00FF zeigt, muss dieses verhalten hier nachgebaut werden

	if(ptr_lo == 0x00FF) 
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	} else { // So soll es sein ;)
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0); // Adresswerte aus dem Bus lesen an position des Pointers
	}
	return 0;
}

uint8_t olc6502::IZX() { // Indirect X mode
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t olc6502::IZY() { // Indirect Y mode (dieser modus funktioniert anders als der IZX!
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t+1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	}
	return 0;
}
#pragma endregion

#pragma region Instructions
uint8_t olc6502::fetch()
{
	if (!(lookup[opcode].addrmode == &olc6502::IMP)) { // alle außer IMP Mode 
		fetched = read(addr_abs);
	}

	return fetched;
}

uint8_t olc6502::AND() {
	fetch(); // Daten lesen
	a = a & fetched; // Bitwise AND mit A Register
	setFlag(Z, a == 0x00); // Zero Flag setzen, wenn a = 0x00
	setFlag(N, a & 0x80); // wenn bit 7 = 1 -> Negativ 
	return 1; // Benötigt einen weiteren Takt
}

uint8_t olc6502::BCS() { // Branch if Carry Set (wenn C Flag = 1)
	if (getFlag(C) == 1) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) { // Bei Pagewechsel einen weiteren Takt
			cycles++; 
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::ASL() //Instruction: Arithmetic Shift Left
{
	fetch();
	temp = (uint16_t)fetched << 1;
	setFlag(C, (temp & 0xFF00) > 0);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::BCC() { //  Branch if Carry Clear (wenn C Flag = 0)
	if (getFlag(C) == 0) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BEQ() { // Branch if Equal (Z Flag = 1)
	if (getFlag(Z) == 1) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BIT()
{
	fetch();
	temp = a & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

uint8_t olc6502::BMI() { // Branch if Negative (wenn N Flag = 1)
	if (getFlag(Z) == 1) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BNE() { //  Branch if Not Equal (wenn Z Flag = 0)
	if (getFlag(Z) == 0) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BPL() { //  Branch if Positive (wenn N Flag = 0)
	if (getFlag(N) == 0) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BRK() // Break
{
	pc++;

	setFlag(I, 1);
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	setFlag(B, 1);
	write(0x0100 + stkp, status);
	stkp--;
	setFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

uint8_t olc6502::BVC() { //  Branch if Overflow Clear (wenn V Flag = 0)
	if (getFlag(V) == 0) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::BVS() { //  Branch if Overflow Set (wenn V Flag = 1)
	if (getFlag(V) == 1) {
		cycles++; // Braucht einen weiteren Takt
		addr_abs = pc + addr_rel; // Absolute Adresse auf PC + relative Adresse setzen

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {// Bei Pagewechsel einen weiteren Takt
			cycles++;
		}

		pc = addr_abs; // Da es ein Branch (JUMP) wird der PC auf die nächste Adresse gesetzt
	}
	return 0;
}

uint8_t olc6502::CLC() // Clear Carry Flag
{
	setFlag(C, false);
	return 0;
}

uint8_t olc6502::CLD() // Clear Decimal Flag
{
	setFlag(D, false);
	return 0;
}

uint8_t olc6502::CLI() // Clear Interrupt Flag
{
	setFlag(I, false);
	return 0;
}

uint8_t olc6502::CLV() // Clear Overflow Flag
{
	setFlag(V, false);
	return 0;
}

uint8_t olc6502::CMP() //Compare Accumulator
{
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	setFlag(C, a >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, (temp) & 0x0080);
	return 1;
}

uint8_t olc6502::CPX() // Compare X Register
{
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	setFlag(C, x >= fetched);
	setFlag(Z, (temp * 0x00FF) == 0x0000);
	setFlag(N, (temp) & 0x0080);
	return 0;
}

uint8_t olc6502::CPY()
{
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	setFlag(C, y >= fetched);
	setFlag(Z, (temp * 0x00FF) == 0x0000);
	setFlag(N, (temp) & 0x0080);
	return 0;
}

uint8_t olc6502::DEC()
{
	return uint8_t();
}

uint8_t olc6502::ADC()
{
	fetch();
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)getFlag(C);
	setFlag(C, temp > 255); // Wenn zahl > 255 -> Carry Bit setzen
	setFlag(Z, (temp & 0x00FF) == 0); // Wenn die Zahl = 0 ist Zero Flag setzen
	setFlag(N, (temp & 0x80)); // wenn das 7 Bit = 1 -> Negativ Flag setzen

	// Overflow ist etwas komplexer. Basierend auf folgender Tabelle:
	//       Positive Number + Positive Number = Negative Result -> Overflow
	//       Negative Number + Negative Number = Positive Result -> Overflow
	//       Positive Number + Negative Number = Either Result -> Cannot Overflow
	//       Positive Number + Positive Number = Positive Result -> OK! No Overflow
	//       Negative Number + Negative Number = Negative Result -> OK! No Overflow
	//
	// und folgender Wahrheitstabelle:
	// A  M  R | V | A^R | A^M |~(A^M) | 
	// 0  0  0 | 0 |  0  |  0  |   1   |
	// 0  0  1 | 1 |  1  |  0  |   1   |
	// 0  1  0 | 0 |  0  |  1  |   0   |
	// 0  1  1 | 0 |  1  |  1  |   0   |  so V = ~(A^M) & (A^R)
	// 1  0  0 | 0 |  1  |  1  |   0   |
	// 1  0  1 | 0 |  0  |  1  |   0   |
	// 1  1  0 | 1 |  1  |  0  |   1   |
	// 1  1  1 | 0 |  0  |  0  |   1   |

	// geht die folgende Zeile aus
	setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

	a = temp & 0x00FF; // Ergebnis wieder ins A Register schreiben
	return 1;
}

uint8_t olc6502::RTI()
{
	stkp++;
	status = read(0x0100 + stkp);
	status &= ~B;
	status &= ~U;

	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;

	return 0;

}

uint8_t olc6502::SBC()
{
	fetch();


	// das untereg byte muss für die Subtraction invertiert werden
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// durch das invertieren ist es quasi eine Adition
	temp = (uint16_t)a + value + (uint16_t)getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, ((temp & 0x00FF) == 0));
	setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	setFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

uint8_t olc6502::PHA() { //Push Accumulator to Stack
	write(0x0100 + stkp, a); //0x0100 ist die Stack Adresse
	stkp--; // Stackpointer -1
	return 0;
}

uint8_t olc6502::PLA() {// Pop Accumulator off Stack
	stkp++; // Stackpointer +1
	a = read(0x0100 + stkp); // Daten lesen
	setFlag(Z, a == 0x00); // Wenn Zero -> Z Flag setzen
	setFlag(N, a & 0x80); // Wenn Negativ N Flag setzen

	return 0;
}


#pragma endregion


