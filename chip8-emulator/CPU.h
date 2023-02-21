#pragma once

#include "Exceptions.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <exception>
#include <fstream>
#include <random>
#include <string>

class CPU
{
public:
	CPU(std::string romFilename);
	~CPU();
	void cycle();
	uint32_t* getVideoMemory();
private:
	constexpr static int memorySize = 4096;
	const uint16_t romStartAddress = 0x200;
	const int maxRomSize = 3584;
	constexpr static int fontsetSize = 80;
	const uint16_t fontStartAddress = 0x50;
	const uint8_t fontset[fontsetSize] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	uint8_t registers[16]{};
	uint8_t* memory;
	uint32_t* videoMemory;
	uint16_t indexRegister{ 0 };
	uint16_t programCounter{ romStartAddress };
	uint16_t stack[16]{};
	uint8_t stackPointer{ 0 };
	uint8_t delayTimer{ 0 };
	uint8_t soundTimer{ 0 };
	std::default_random_engine randEngine;
	std::uniform_int_distribution<uint16_t> randInt;
	bool input[16]{};
	void loadROM(std::string filename);
	void loadFontset();
	void decodeOpcode(uint16_t opcode);
	void OP_00E0();
	void OP_00EE();
	void OP_1nnn(uint16_t opcode);
	void OP_2nnn(uint16_t opcode);
	void OP_3xkk(uint16_t opcode);
	void OP_4xkk(uint16_t opcode);
	void OP_5xy0(uint16_t opcode);
	void OP_6xkk(uint16_t opcode);
	void OP_7xkk(uint16_t opcode);
	void OP_8xy0(uint16_t opcode);
	void OP_8xy1(uint16_t opcode);
	void OP_8xy2(uint16_t opcode);
	void OP_8xy3(uint16_t opcode);
	void OP_8xy4(uint16_t opcode);
	void OP_8xy5(uint16_t opcode);
	void OP_8xy6(uint16_t opcode);
	void OP_8xy7(uint16_t opcode);
	void OP_8xyE(uint16_t opcode);
	void OP_9xy0(uint16_t opcode);
	void OP_Annn(uint16_t opcode);
	void OP_Bnnn(uint16_t opcode);
	void OP_Cxkk(uint16_t opcode);
	void OP_Dxyn(uint16_t opcode);
	void OP_Ex9E(uint16_t opcode);
	void OP_ExA1(uint16_t opcode);
};
