#include <chip8/Chip8.h>

int main()
{
	Chip8 chip8 = Chip8();
	while (true)
	{
		chip8.Clock();
	}
	return 0;
}