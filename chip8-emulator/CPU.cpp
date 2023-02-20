#include "CPU.h"

CPU::CPU(std::string romFilename) :
	memory{ new uint8_t[4096]{} },
	videoMemory{ new uint32_t[64 * 32]{} }
{
	std::ifstream romFile = std::ifstream(romFilename, std::ios::binary);
	romFile.close();
}

CPU::~CPU()
{
	delete[] memory;
	delete[] videoMemory;
}

uint32_t* CPU::getVideoMemory()
{
	return videoMemory;
}
