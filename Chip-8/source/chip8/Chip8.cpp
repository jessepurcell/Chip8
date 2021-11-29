#include <chip8/Chip8.h>

#include <iostream>

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
	memory.SetByte(0x201, 0x00);
}

void Chip8::Clock()
{
	Decode();
	Execute();
}

bit16 Chip8::Fetch()
{
	return bit16();
}

void Chip8::Decode()
{
	bit16 instruction = memory.GetByte(cpu.PC) << 8 | memory.GetByte(cpu.PC + 1);

	switch (instruction & 0xF000)
	{
		switch (instruction & 0x000F)
		{
		case 0x0000:// 0x0000 -> Clear screen
			//To-Do: Clear screen
			break;
		case 0x000E:// 0x00EE: Return from subroutine
			//To-Do: Return from subroutine
			break;
		}
	case 0x1000:
	{
		cpu.PC = instruction & 0x0FFF;
		break;
	}
	case 0xA000: //Annn -> Sets I to the address nnn
	{
		cpu.I = instruction & 0x0FFF;
		cpu.PC += 2;
		break;
	}
	default:
		printf("Unknown instruction: 0x%X\n", instruction);
		system("pause");
		break;
	}

	printf("Executing instruction: 0x%X\n", instruction);
}

void Chip8::Execute()
{

}

bit16 Chip8::GetInstruction()
{
	return memory.GetByte(cpu.PC) << 8 | memory.GetByte(cpu.PC + 1);
}

bit8 Chip8::GetOpcode()
{
	bit16 instruction = GetInstruction();
	return instruction >> 12;
}

bit8 Chip8::GetArg1()
{
	bit16 ins = GetInstruction() & 0000111111111111;
	return ins;
}

bit8 Chip8::GetArg2()
{
	return GetInstruction() << 4 >> 12;
}
