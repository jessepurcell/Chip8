#include <chip8/CPU.h>

int main()
{
	CPU *cpu = new CPU();
	cpu->Initialize();
	cpu->LoadROM("C:\\Users\\slayd\\source\\repos\\Miosyo\\Emulators\\Chip-8\\roms\\demos\\Maze [David Winter, 199x].ch8");

	while (true)
	{
		cpu->Cycle();
	}
	delete cpu;
	return 0;
}