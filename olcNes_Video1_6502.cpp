/*
	olc6502 - An emulation of the 6502/2A03 processor
	"Thanks Dad for believing computers were gonna be a big deal..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Background
	~~~~~~~~~~
	I love this microprocessor. It was at the heart of two of my favourite
	machines, the BBC Micro, and the Nintendo Entertainment System, as well
	as countless others in that era. I learnt to program on the Model B, and
	I learnt to love games on the NES, so in many ways, this processor is
	why I am the way I am today.

	In February 2019, I decided to undertake a selfish personal project and
	build a NES emulator. Ive always wanted to, and as such I've avoided
	looking at source code for such things. This made making this a real
	personal challenge. I know its been done countless times, and very likely
	in far more clever and accurate ways than mine, but I'm proud of this.

	Datasheet: http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

	Files: olc6502.h, olc6502.cpp

	Relevant Video: https://youtu.be/8XmxKPJDGU0

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019
*/

#include <iostream>
#include <sstream>

#include "Bus.h"
#include "olc6502.h"
//#include "Cartridge.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Demo_olc6502 : public olc::PixelGameEngine
{
public:
	Demo_olc6502() { sAppName = "olc6502 Demonstration"; }

	std::shared_ptr<Cartridge> cart;
	Bus nes;
	std::map<uint16_t, std::string> mapAsm;

	bool bEmulationRun = false;
	float fResidualTime = 0.0;

	uint8_t nSelectedPalette = 0x00;


	std::string hex(uint32_t wert, uint8_t Stellen)
	{
		std::string s(Stellen, '0');
		for (int i = Stellen - 1; i >= 0; i--, wert >>= 4)
			s[i] = "0123456789ABCDEF"[wert & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x, y, "STATUS:", olc::WHITE);
		DrawString(x + 64, y, "N", nes.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x + 80, y, "V", nes.cpu.status & olc6502::V ? olc::GREEN : olc::RED);
		DrawString(x + 96, y, "-", nes.cpu.status & olc6502::U ? olc::GREEN : olc::RED);
		DrawString(x + 112, y, "B", nes.cpu.status & olc6502::B ? olc::GREEN : olc::RED);
		DrawString(x + 128, y, "D", nes.cpu.status & olc6502::D ? olc::GREEN : olc::RED);
		DrawString(x + 144, y, "I", nes.cpu.status & olc6502::I ? olc::GREEN : olc::RED);
		DrawString(x + 160, y, "Z", nes.cpu.status & olc6502::Z ? olc::GREEN : olc::RED);
		DrawString(x + 178, y, "C", nes.cpu.status & olc6502::C ? olc::GREEN : olc::RED);
		DrawString(x, y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x, y + 20, "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x, y + 30, "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x, y + 40, "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x, y + 50, "Stack P: $" + hex(nes.cpu.stkp, 4));
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	bool OnUserCreate()
	{
		// Lade ein Cartridge
		cart = std::make_shared<Cartridge>("dk.nes");

		// einlegen
		nes.insertCartridge(cart);

		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		nes.reset();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);

		if (bEmulationRun) {

			if (fResidualTime > 0.0f) {
				fResidualTime -= fElapsedTime;
			} else {

				fResidualTime += (1.0f / 60.0f) - fElapsedTime;
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}

		} else {
	
			if (GetKey(olc::Key::C).bPressed) {
	
				// nächste Instruktion ausführen
				do { nes.clock(); } while (!nes.cpu.complete());
				do { nes.clock(); } while (!nes.cpu.complete());
			}
	
			if (GetKey(olc::Key::F).bPressed) {
	
				// nächste Instruktion ausführen
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				do { nes.clock(); } while (!nes.cpu.complete());
	
				nes.ppu.frame_complete = false;
			}
		}

		if (GetKey(olc::Key::R).bPressed) {
			nes.reset();
		}

		if (GetKey(olc::Key::SPACE).bPressed) {
			bEmulationRun = !bEmulationRun;
		}

		if (GetKey(olc::Key::P).bPressed) {
			(++nSelectedPalette) &= 0x07;
		}



		DrawCpu(516, 2);
		DrawCode(516, 72, 26);

		const int nSwatchSize = 6;
		for (int p = 0; p < 8; p++) { // Palette
			for (int s = 0; s < 4; s++) { // Index der Palette
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, nSwatchSize, nSwatchSize, nes.ppu.GetColorFromPaletteRam(p, s));
			}
		}

		DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

		DrawSprite(516, 348, &nes.ppu.GetPatternTable(0, nSelectedPalette));
		DrawSprite(648, 348, &nes.ppu.GetPatternTable(1, nSelectedPalette));

		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);


		//// Die erste BG Grafik wird angezeigt!!! ;)
		//olc::Sprite& s = nes.ppu.GetPatternTable(1, nSelectedPalette);
		//for (uint8_t y = 0; y < 30; y++) {
		//	for (uint8_t x = 0; x < 32; x++) {
		//		uint8_t id = (uint8_t)nes.ppu.tblName[0][y * 32 + x];
		//		DrawPartialSprite(x * 16, y * 16, &s, (id & 0x0F) << 3, ((id >> 4) & 0x0F) << 3, 8,8,2);
		//	}
		//}

		return true;
	}
};


int main()
{
	Demo_olc6502 demo;
	demo.Construct(780, 480, 2, 2);
	demo.Start();
	return 0;
}