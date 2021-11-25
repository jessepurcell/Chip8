#include <chip8/Memory.h>

Memory::Memory()
{
	for (int i = 0; i < BLOCKS; i++)
	{
		data[i] = 0x00000000;
	}
}

Memory::~Memory()
{
}

void Memory::SetByte(bit16 addr, bit8 data)
{
	Memory::data[addr] = data;
}

bit8 Memory::GetByte(bit16 addr)
{
	return data[addr];
}
