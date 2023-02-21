#include "CPU.h"

CPU::CPU(std::string romFilename) :
	memory{ new uint8_t[4096]{} },
	videoMemory{ new uint32_t[64 * 32]{} }
{
	loadROM(romFilename);
	loadFontset();
}

void CPU::loadROM(std::string filename)
{
	std::ifstream romFile = std::ifstream(filename, std::ifstream::binary | std::ifstream::ate);
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
		for (int romIndex = 0; romIndex < fileSize; ++romIndex)
		{
			memory[romStartAddress + romIndex] = buffer[romIndex];
		}
		delete[] buffer;
	}
	else
	{
		throw FileNotFoundException(filename);
	}
	romFile.close();
}

void CPU::loadFontset()
{
	for (int fontIndex = 0; fontIndex < fontsetSize; ++fontIndex)
	{
		memory[fontStartAddress + fontIndex] = fontset[fontIndex];
	}
}

CPU::~CPU()
{
	delete[] memory;
	delete[] videoMemory;
}

void CPU::cycle()
{
	uint16_t opcode = (memory[programCounter] << 8) | memory[programCounter + 1];
	decodeOpcode(opcode);
	programCounter += 2;
	if (delayTimer > 0) --delayTimer;
	if (soundTimer > 0) --soundTimer;
}

void CPU::decodeOpcode(uint16_t opcode)
{
	uint8_t part1 = (opcode & 0xF000) >> 12;
	uint8_t part2 = (opcode & 0x0F00) >> 8;
	uint8_t part3 = (opcode & 0x00F0) >> 4;
	uint8_t part4 = opcode & 0x000F;
}

uint32_t* CPU::getVideoMemory()
{
	return videoMemory;
}
