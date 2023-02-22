#include "CPU.h"

CPU::CPU(std::string romFilename) :
	memory{ new uint8_t[memorySize]{} },
	videoMemory{ new uint32_t[64 * 32]{} },
	randEngine{ std::chrono::system_clock::now().time_since_epoch().count() },
	randInt{std::uniform_int_distribution<uint16_t>(0, 255)}
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
	programCounter += 2;
	decodeOpcode(opcode);
	if (delayTimer > 0) --delayTimer;
	if (soundTimer > 0) --soundTimer;
}

void CPU::decodeOpcode(uint16_t opcode)
{
	uint8_t part1 = (opcode & 0xF000) >> 12;
	uint8_t part2 = (opcode & 0x0F00) >> 8;
	uint8_t part3 = (opcode & 0x00F0) >> 4;
	uint8_t part4 = opcode & 0x000F;
	switch (part1)
	{
	case 0x0:
		switch (part4)
		{
		case 0x0:
			OP_00E0();
			break;
		case 0xE:
			OP_00EE();
			break;
		}
		break;
	case 0x1:
		OP_1nnn(opcode);
		break;
	case 0x2:
		OP_2nnn(opcode);
		break;
	case 0x3:
		OP_3xkk(opcode);
		break;
	case 0x4:
		OP_4xkk(opcode);
		break;
	case 0x5:
		OP_5xy0(opcode);
		break;
	case 0x6:
		OP_6xkk(opcode);
		break;
	case 0x7:
		OP_7xkk(opcode);
		break;
	case 0x8:
		switch (part4)
		{
		case 0x0:
			OP_8xy0(opcode);
			break;
		case 0x1:
			OP_8xy1(opcode);
			break;
		case 0x2:
			OP_8xy2(opcode);
			break;
		case 0x3:
			OP_8xy3(opcode);
			break;
		case 0x4:
			OP_8xy4(opcode);
			break;
		case 0x5:
			OP_8xy5(opcode);
			break;
		case 0x6:
			OP_8xy6(opcode);
			break;
		case 0x7:
			OP_8xy7(opcode);
			break;
		case 0xE:
			OP_8xyE(opcode);
			break;
		}
		break;
	case 0x9:
		OP_9xy0(opcode);
		break;
	case 0xA:
		OP_Annn(opcode);
		break;
	case 0xB:
		OP_Bnnn(opcode);
		break;
	case 0xC:
		OP_Cxkk(opcode);
		break;
	case 0xD:
		OP_Dxyn(opcode);
		break;
	case 0xE:
		switch (part4)
		{
		case 0xE:
			OP_Ex9E(opcode);
			break;
		case 0x1:
			OP_ExA1(opcode);
			break;
		}
		break;
	case 0xF:
		if (part3 == 0x0 && part4 == 0x7) OP_Fx07(opcode);
		if (part3 == 0x0 && part4 == 0xA) OP_Fx0A(opcode);
		if (part3 == 0x1 && part4 == 0x5) OP_Fx15(opcode);
		if (part3 == 0x1 && part4 == 0xE) OP_Fx1E(opcode);
		if (part3 == 0x2 && part4 == 0x9) OP_Fx29(opcode);
		if (part3 == 0x3 && part4 == 0x3) OP_Fx33(opcode);
		if (part3 == 0x5 && part4 == 0x5) OP_Fx55(opcode);
		if (part3 == 0x6 && part4 == 0x5) OP_Fx65(opcode);
		break;
	}
}

uint32_t* CPU::getVideoMemory()
{
	return videoMemory;
}

void CPU::setInput(int index, bool value)
{
	input[index] = value;
}

void CPU::OP_00E0()
{
	std::fill(videoMemory, videoMemory + 4096, 0);
}

void CPU::OP_00EE()
{
	--stackPointer;
	programCounter = stack[stackPointer];
}

void CPU::OP_1nnn(uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFF;
	programCounter = address;
}

void CPU::OP_2nnn(uint16_t opcode)
{
	stack[stackPointer] = programCounter;
	++stackPointer;
	uint16_t address = opcode & 0x0FFF;
	programCounter = address;
}

void CPU::OP_3xkk(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (registers[vx] == value) programCounter += 2;
}

void CPU::OP_4xkk(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (registers[vx] != value) programCounter += 2;
}

void CPU::OP_5xy0(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	if (registers[vx] == registers[vy]) programCounter += 2;
}

void CPU::OP_6xkk(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	registers[vx] = value;
}

void CPU::OP_7xkk(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	registers[vx] += value;
}

void CPU::OP_8xy0(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	registers[vx] = registers[vy];
}

void CPU::OP_8xy1(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	registers[vx] |= registers[vy];
}

void CPU::OP_8xy2(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	registers[vx] &= registers[vy];
}

void CPU::OP_8xy3(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	registers[vx] ^= registers[vy];
}

void CPU::OP_8xy4(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	uint16_t sum = registers[vx] + registers[vy];
	if (sum > 0xFF) registers[0xF] = 1;
	else registers[0xF] = 0;
	registers[vx] = sum;
}

void CPU::OP_8xy5(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	bool carryFlag = false;
	if (registers[vx] > registers[vy]) carryFlag = true;
	registers[vx] -= registers[vy];
	if (carryFlag) registers[0xF] = 1;
	else registers[0xF] = 0;
}

void CPU::OP_8xy6(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t carryFlag = registers[vx] & 0x0001;
	registers[vx] >>= 1;
	registers[0xF] = carryFlag;
}

void CPU::OP_8xy7(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	bool carryFlag = false;
	if (registers[vx] < registers[vy]) carryFlag = true;
	registers[vx] = registers[vy] - registers[vx];
	if (carryFlag) registers[0xF] = 1;
	else registers[0xF] = 0;
}

void CPU::OP_8xyE(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t carryFlag = (registers[vx] & 0x80) >> 7;
	registers[vx] <<= 1;
	registers[0xF] = carryFlag;
}

void CPU::OP_9xy0(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	if (registers[vx] != registers[vy]) programCounter += 2;
}

void CPU::OP_Annn(uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFF;
	indexRegister = address;
}

void CPU::OP_Bnnn(uint16_t opcode)
{
	uint16_t address = opcode & 0x0FFF;
	address += registers[0];
	programCounter = address;
}

void CPU::OP_Cxkk(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	registers[vx] = randInt(randEngine) & value;
}

void CPU::OP_Dxyn(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	uint8_t height = opcode & 0x000F;
	uint8_t posX = registers[vx] % 64;
	uint8_t posY = registers[vy] % 32;
	registers[0xF] = 0;
	for (int row = 0; row < height; ++row)
	{
		uint8_t spriteRow = memory[indexRegister + row];
		for (int column = 0; column < 8; ++column)
		{
			uint8_t spritePixel = spriteRow & (0x80 >> column);
			uint32_t* screenPixel = &videoMemory[(posY + row) * 64 + (posX + column)];
			if (spritePixel)
			{
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void CPU::OP_Ex9E(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[vx];
	if (input[key]) programCounter += 2;
}

void CPU::OP_ExA1(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t key = registers[vx];
	if (!input[key]) programCounter += 2;
}

void CPU::OP_Fx07(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	registers[vx] = delayTimer;
}

void CPU::OP_Fx0A(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	if (input[0]) registers[vx] = 0;
	else if (input[1]) registers[vx] = 1;
	else if (input[2]) registers[vx] = 2;
	else if (input[3]) registers[vx] = 3;
	else if (input[4]) registers[vx] = 4;
	else if (input[5]) registers[vx] = 5;
	else if (input[6]) registers[vx] = 6;
	else if (input[7]) registers[vx] = 7;
	else if (input[8]) registers[vx] = 8;
	else if (input[9]) registers[vx] = 9;
	else if (input[10]) registers[vx] = 10;
	else if (input[11]) registers[vx] = 11;
	else if (input[12]) registers[vx] = 12;
	else if (input[13]) registers[vx] = 13;
	else if (input[14]) registers[vx] = 14;
	else if (input[15]) registers[vx] = 15;
	else programCounter -= 2;
}

void CPU::OP_Fx15(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	delayTimer = registers[vx];
}

void CPU::OP_Fx1E(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	indexRegister += registers[vx];
}

void CPU::OP_Fx29(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t digit = registers[vx];
	indexRegister = fontStartAddress + (5 * digit);
}

void CPU::OP_Fx33(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[vx];
	memory[indexRegister + 2] = value % 10;
	value /= 10;
	memory[indexRegister + 1] = value % 10;
	value /= 10;
	memory[indexRegister] = value % 10;
}

void CPU::OP_Fx55(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	for (int reg = 0; reg <= vx; ++reg)
	{
		memory[indexRegister + reg] = registers[reg];
	}
}

void CPU::OP_Fx65(uint16_t opcode)
{
	uint8_t vx = (opcode & 0x0F00) >> 8;
	for (int reg = 0; reg <= vx; ++reg)
	{
		registers[reg] = memory[indexRegister + reg];
	}
}
