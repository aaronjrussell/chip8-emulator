#pragma once

#include <cstdint>
#include <fstream>
#include <string>

class CPU
{
public:
	CPU(std::string romFilename);
	~CPU();
	void cycle();
	uint32_t* getVideoMemory();
private:
	uint8_t registers[16]{};
	uint8_t* memory;
	uint32_t* videoMemory;
	uint16_t indexRegister{ 0 };
	uint16_t programCounter{ 0 };
	uint16_t stack[16]{};
	uint8_t stackPointer{ 0 };
	uint8_t delayTimer{ 0 };
	uint8_t soundTimer{ 0 };
	bool input[16]{};
};
