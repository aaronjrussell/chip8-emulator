#include "CPU.h"

CPU::CPU(std::string romFilename)
{
	std::ifstream romFile = std::ifstream(romFilename, std::ios::binary);
	romFile.close();
}
