#pragma once

#include <cstdint>
#include <fstream>
#include <string>

class CPU
{
public:
	CPU(std::string romFilename);
private:
	uint8_t registers[16]{ 0 };
	uint8_t memory[4096]{ 0 };
	uint32_t videoMemory[64 * 32]{ 0 };
	uint16_t indexRegister{ 0 };
	uint16_t programCounter{ 0 };
	uint16_t stack[16]{ 0 };
	uint8_t stackPointer{ 0 };
	uint8_t delayTimer{ 0 };
	uint8_t soundTimer{ 0 };
	bool input[16]{ false };
};
