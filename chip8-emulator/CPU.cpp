#include "CPU.h"

CPU::CPU(std::string romFilename) :
	memory{ new uint8_t[4096]{} },
	videoMemory{ new uint32_t[64 * 32]{} }
{
	std::ifstream romFile = std::ifstream(romFilename, std::ifstream::binary | std::ifstream::ate);
	int fileSize = romFile.tellg();
	if (fileSize > maxRomSize)
	{
		romFile.close();
		throw RomTooLargeException();
	}
	romFile.seekg(0);
	if (romFile.is_open())
	{
		char* buffer = new char[fileSize];
		romFile.read(buffer, fileSize);
		for (int romIndex = 0; romIndex < fileSize; romIndex++)
		{
			memory[romStartAddress + romIndex] = buffer[romIndex];
		}
		delete[] buffer;
	}
	else
	{
		throw FileNotFoundException(romFilename);
	}
	romFile.close();
}

CPU::~CPU()
{
	delete[] memory;
	delete[] videoMemory;
}

void CPU::cycle()
{}

uint32_t* CPU::getVideoMemory()
{
	return videoMemory;
}
