#include <chip8/Chip8.h>

Chip8::Chip8()
{
	cpu = CPU();
	memory = Memory();
	Initialize();
}

Chip8::~Chip8()
{
}

void Chip8::Initialize()
{
	memory.SetByte(0x200, 0x12);
	memory.SetByte(0x201, 0x34);
}

void Chip8::Clock()
{
	Decode();
	Execute();
}

void Chip8::Decode()
{
	bit16 instruction = memory.GetByte(cpu.PC) << 8 | memory.GetByte(cpu.PC + 1);
	bit16 opcode = instruction >> 12;
	//opcode << memory.GetByte(cpu.PC);
	//opcode << memory.GetByte(cpu.PC + 1);
}

void Chip8::Execute()
{

}
