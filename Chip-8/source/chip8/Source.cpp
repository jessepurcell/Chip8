/*
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
}*/

#include <chip8/CPU.h>
#include <SFML/Graphics.hpp>

#define SCALE 16

int main()
{
    CPU* cpu = new CPU();
    cpu->Initialize();
    cpu->LoadROM("C:\\Users\\slayd\\source\\repos\\Miosyo\\Emulators\\Chip-8\\roms\\programs\\chip8-test-rom.ch8");

    sf::RenderWindow window(sf::VideoMode(64 * SCALE, 32 * SCALE), "Chip-8");
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(64 * SCALE, 32 * SCALE)) {};

    sf::Uint8* pixels = new sf::Uint8[64 * 32 * 4];
    sf::Texture texture;
    texture.create(64, 32);
    sf::Sprite sprite(texture);
    for (int i = 0; i < 64 * 32 * 4; i += 4)
    {
        pixels[i] = 0;//red;
        pixels[i + 1] = 00; //red;
        pixels[i + 2] = 0;//red;
        pixels[i + 3] = 255;//red;
    }
    texture.update(pixels);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        cpu->Cycle();

        for (int i = 0; i < 64 * 32 * 4; i += 4)
        {
            pixels[i] = 0;//red;
            pixels[i + 1] = cpu->video[i/4] * 255; //green;
            pixels[i + 2] = 0;//blue;
            pixels[i + 3] = 255;//alpha;
        }
        texture.update(pixels);

        renderTexture.clear();
        //Draw
        renderTexture.draw(sprite);
        renderTexture.display();

        const sf::Texture& texture = renderTexture.getTexture();
        sf::Sprite sprite(texture);
        sprite.setScale(sf::Vector2f(SCALE, SCALE));
        window.draw(sprite);
        window.display();
    }

    return 0;
}