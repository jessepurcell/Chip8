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

	void Decode();
	void Execute();
};