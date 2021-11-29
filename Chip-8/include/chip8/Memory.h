#pragma once
#include <chip8/Data.h>

#define MEM_SIZE 0x1000

class Memory
{
private:
	//Set memory to 4kb
	bit8 data[MEM_SIZE];
public:
	Memory();
	~Memory();
	void SetByte(bit16 addr, bit8 data);
	bit8 GetByte(bit16 addr);
};