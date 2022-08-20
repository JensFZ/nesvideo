#pragma once
#include <cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

protected:
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;
};

