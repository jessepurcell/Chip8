#include <chip8/CPU.h>

void CPU::Initialize()
{
	for (int i = 0; i < 16; i++)
	{
		V[i] = 0x0;
		stack[i] = 0x0;
		keypad[i] = false;
	}
	I = 0x0;
	sound = 0x0;
	delay = 0x0;
	PC = 0x200;
	SP = 0x0;
	memory = Memory();

	for (int i = 0; i < FONTSET_SIZE; i++)
	{
		memory.SetByte(0x80 + i, fontset[i]);
	}

	LoadOpcodes();

	for (int i = 0; i < 64 * 32; i++)
	{
		video[i] = 0;
	}

	opcode = 0xFFFF;

	rng = std::mt19937(std::random_device{}());
	dist = std::uniform_int_distribution<>(0, 255);
}

void CPU::LoadOpcodes()
{
	table[0x0] = &CPU::Table0;
	table[0x1] = &CPU::OP_1nnn;
	table[0x2] = &CPU::OP_2nnn;
	table[0x3] = &CPU::OP_3xkk;
	table[0x4] = &CPU::OP_4xkk;
	table[0x5] = &CPU::OP_5xy0;
	table[0x6] = &CPU::OP_6xkk;
	table[0x7] = &CPU::OP_7xkk;
	table[0x8] = &CPU::Table8;
	table[0x9] = &CPU::OP_9xy0;
	table[0xA] = &CPU::OP_Annn;
	table[0xB] = &CPU::OP_Bnnn;
	table[0xC] = &CPU::OP_Cxkk;
	table[0xD] = &CPU::OP_Dxyn;
	table[0xE] = &CPU::TableE;
	table[0xF] = &CPU::TableF;

	table0[0x0] = &CPU::OP_00E0;
	table0[0xE] = &CPU::OP_00EE;

	table8[0x0] = &CPU::OP_8xy0;
	table8[0x1] = &CPU::OP_8xy1;
	table8[0x2] = &CPU::OP_8xy2;
	table8[0x3] = &CPU::OP_8xy3;
	table8[0x4] = &CPU::OP_8xy4;
	table8[0x5] = &CPU::OP_8xy5;
	table8[0x6] = &CPU::OP_8xy6;
	table8[0x7] = &CPU::OP_8xy7;
	table8[0xE] = &CPU::OP_8xyE;

	tableE[0x1] = &CPU::OP_ExA1;
	tableE[0xE] = &CPU::OP_Ex9E;

	tableF[0x07] = &CPU::OP_Fx07;
	tableF[0x0A] = &CPU::OP_Fx0A;
	tableF[0x15] = &CPU::OP_Fx15;
	tableF[0x18] = &CPU::OP_Fx18;
	tableF[0x1E] = &CPU::OP_Fx1E;
	tableF[0x29] = &CPU::OP_Fx29;
	tableF[0x33] = &CPU::OP_Fx33;
	tableF[0x55] = &CPU::OP_Fx55;
	tableF[0x65] = &CPU::OP_Fx65;
}

void CPU::Table0()
{
	((*this).*(table0[opcode & 0x000F]))();
}

void CPU::Table8()
{
	((*this).*(table8[opcode & 0x000F]))();
}

void CPU::TableE()
{
	((*this).*(tableE[opcode & 0x000F]))();
}

void CPU::TableF()
{
	((*this).*(tableF[opcode & 0x00FF]))();
}

void CPU::LoadROM(char const* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (int i = 0; i < size; ++i)
		{
			memory.SetByte(0x200 + i, (bit8)buffer[i]);
		}
		delete[] buffer;
	}
}

void CPU::Cycle()
{
	Fetch();

	((*this).*(table[(opcode & 0xF000) >> 12]))();

	if (delay > 0) delay--;
	if (sound > 0) sound--;
}

void CPU::Fetch()
{
	opcode = memory.GetByte(PC) << 8 | memory.GetByte(PC + 1);
	printf("OPCODE 0x%04X \n", opcode);
	PC += 2;
}

void CPU::OP_NULL()
{
	printf("Invalid OPCODE 0x%04X", opcode);
}

void CPU::OP_00E0()
{
	for (int i = 0; i < 64 * 32; i++)
	{
		video[i] = false;
	}
}

void CPU::OP_00EE()
{
	SP--;
	PC = stack[SP];
}

void CPU::OP_1nnn()
{
	bit16 address = opcode & 0xFFF;
	PC = address;
}

void CPU::OP_2nnn()
{
	bit16 address = opcode & 0xFFF;
	stack[SP] = PC;
	SP++;
	PC = address;
}

void CPU::OP_3xkk()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 byte = opcode & 0x00FF;
	if (V[x] == byte) PC += 2;
}

void CPU::OP_4xkk()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 byte = opcode & 0x00FF;
	if (V[x] != byte) PC += 2;
}

void CPU::OP_5xy0()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	if (V[x] == V[y]) PC += 2;
}

void CPU::OP_6xkk()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 byte = opcode & 0x00FF;
	V[x] = byte;
}

void CPU::OP_7xkk()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 byte = opcode & 0x00FF;
	V[x] += byte;
}

void CPU::OP_8xy0()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	V[x] = V[y];
}

void CPU::OP_8xy1()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	V[x] |= V[y];
}

void CPU::OP_8xy2()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	V[x] &= V[y];
}

void CPU::OP_8xy3()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	V[x] ^= V[y];
}

void CPU::OP_8xy4()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;

	bit16 sum = V[x] + V[y];
	if (sum > 0xFF) V[0xF] = 0x1;
	else V[0xF] = 0x0;
	V[x] = sum & 0xFF;
}

void CPU::OP_8xy5()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	if (V[x] > V[y]) V[0xF] = 1;
	else V[0xF] = 0;
	V[x] -= V[y];
}

void CPU::OP_8xy6()
{
	bit8 x = opcode & 0x0F00 >> 8;
	V[0xF] = V[x] & 0x1;
	V[x] >>= 1;
}

void CPU::OP_8xy7()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	if (V[y] > V[x]) V[0xF] = 1;
	else V[0xF] = 0;
	V[x] = V[y] - V[x];
}

void CPU::OP_8xyE()
{
	bit8 x = opcode & 0x0F00 >> 8;
	V[0xF] = V[x] & 0x80 >> 7;
	V[x] <<= 1;
}

void CPU::OP_9xy0()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 y = opcode & 0x00FF >> 4;
	if (V[x] != V[y]) PC += 2;
}

void CPU::OP_Annn()
{
	bit16 address = opcode & 0x0FFF;
	I = address;
}

void CPU::OP_Bnnn()
{
	bit16 address = opcode & 0x0FFF;
	PC = V[0] + address;
}

void CPU::OP_Cxkk()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 byte = opcode & 0x00FF;

	//To-Do: Random number gen
	//Done?
	V[x] = (bit8)dist(rng) & byte;
}

//Doubt this works lol
void CPU::OP_Dxyn()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = V[Vx] % 64;
	uint8_t yPos = V[Vy] % 32;

	V[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory.GetByte(I + row);

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * 64 + (xPos + col)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					V[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

void CPU::OP_Ex9E()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 key = V[x];
	if (keypad[key]) PC += 2;
}

void CPU::OP_ExA1()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 key = V[x];
	if (!keypad[key]) PC += 2;
}

void CPU::OP_Fx07()
{
	bit8 x = opcode & 0x0F00 >> 8;
	V[x] = delay;
}

void CPU::OP_Fx0A()
{
	bit8 x = opcode & 0x0F00 >> 8;

	bool y = false;
	for (int i = 0; i < 16; i++)
	{
		if (keypad[i])
		{
			V[x] = i;
			y = true;
		}
	}
	if (!y) PC -= 2;
}

void CPU::OP_Fx15()
{
	bit8 x = opcode & 0x0F00 >> 8;
	delay = V[x];
}

void CPU::OP_Fx18()
{
	bit8 x = opcode & 0x0F00 >> 8;
	sound = V[x];
}

void CPU::OP_Fx1E()
{
	bit8 x = opcode & 0x0F00 >> 8;
	I += V[x];
}

void CPU::OP_Fx29()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 digit = V[x];
	I = 0x50 + (5 * digit);
}

void CPU::OP_Fx33()
{
	bit8 x = opcode & 0x0F00 >> 8;
	bit8 value = V[x];

	memory.SetByte(I + 2, value % 10);
	value /= 10;

	memory.SetByte(I + 1, value % 10);
	value /= 10;

	memory.SetByte(I, value % 10);
}

void CPU::OP_Fx55()
{
	bit8 x = opcode & 0x0F00 >> 8;

	//This might need to be ++i NOT i++
	for (int i = 0; i < x; i++)
	{
		memory.SetByte(I + i, V[i]);
	}
}

void CPU::OP_Fx65()
{
	bit8 x = opcode & 0x0F00 >> 8;

	for (int i = 0; i < x; i++)
	{
		V[i] = memory.GetByte(I + i);
	}
}
