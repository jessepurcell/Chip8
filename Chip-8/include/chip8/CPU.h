#pragma once
#include <chip8/Data.h>

class CPU
{
public:
	//registers 0->16
	bit8 registers[16];
	//memory location
	bit16 I = 0x0;
	//program counter
	bit16 PC = 0x200;
	//stack pointer
	bit8 SP = 0x0;
	//Delay timer
	bit8 DT = 0x0;
	//sound timer
	bit8 ST = 0x0;
	//Stack
	bit16 stack[16];
public:
	CPU();
	~CPU();

	//Clear display
	void CLS();
	//Return from subroutine
	void RET();
	//Jump ADDRESS
	void JP(bit16 addr);
	//Call ADDRESS
	void CALL(bit16 addr);
	//skip if if Register Vx == kk
	//The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
	void SE(bit8 reg, bit8 val);
	//Skip next instruction if Vx != kk.
	//The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
	void SNE(bit8 reg, bit8 val);
};