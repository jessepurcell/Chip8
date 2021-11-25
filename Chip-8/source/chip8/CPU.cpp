#include <chip8/CPU.h>

CPU::CPU()
{
	for (int i = 0; i < 16; i++)
	{
		registers[i] = 0x0;
	}
}

CPU::~CPU()
{

}