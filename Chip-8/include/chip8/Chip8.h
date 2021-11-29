#include <chip8/CPU.h>
#include <chip8/Memory.h>

class Chip8
{
private:
	CPU cpu;
	Memory memory;
public:
	Chip8();
	~Chip8();

	void Initialize();
	void Clock();

	bit16 Fetch();
	void Decode();
	void Execute();



	bit16 GetInstruction();
	bit8 GetOpcode();
	bit8 GetArg1();
	bit8 GetArg2();
};