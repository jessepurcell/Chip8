#pragma once
#include <fstream>
#include <chrono>
#include <random>

#include <chip8/Data.h>
#include <chip8/Memory.h>

#define FONTSET_SIZE 80
#define SCREEN_WIDTH 64

class CPU
{
private:
	bit8 V[16];
	bit16 I;
	bit8 delay;
	bit16 PC;
	bit8 SP;
	bit16 stack[16];
	Memory memory;

	bit8 fontset[FONTSET_SIZE] =
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

	bit16 opcode;

	typedef void (CPU::*OpcodeFunction)();
	OpcodeFunction table[0xF + 1]{ &CPU::OP_NULL };
	OpcodeFunction table0[0xE + 1]{ &CPU::OP_NULL };
	OpcodeFunction table8[0xE + 1]{ &CPU::OP_NULL };
	OpcodeFunction tableE[0xE + 1]{ &CPU::OP_NULL };
	OpcodeFunction tableF[0x65 + 1]{ &CPU::OP_NULL };

	std::mt19937 rng;
	std::uniform_int_distribution<> dist;
public:
	uint32_t video[64 * 32];
	bit8 sound;
	bool keypad[16];
	bool draw = false;
public:
	void Initialize();
	void LoadROM(char const* filename);
	void Cycle();
private:
	void Fetch();
	void LoadOpcodes();

	void Table0();
	void Table8();
	void TableE();
	void TableF();

	void OP_NULL();

	void OP_00E0();
	void OP_00EE();

	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();

	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();

	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();

	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();
};