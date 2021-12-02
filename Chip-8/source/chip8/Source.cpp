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

#include <thread>

#include <chip8/CPU.h>
#include <SFML/Graphics.hpp>

#define SCALE 32

int main()
{
    CPU* cpu = new CPU();
    cpu->Initialize();
    cpu->LoadROM("C:\\Users\\slayd\\source\\repos\\Miosyo\\Emulators\\Chip-8\\roms\\programs\\BMP Viewer - Hello (C8 example) [Hap, 2005].ch8");

    sf::RenderWindow window(sf::VideoMode(64 * SCALE, 32 * SCALE), "Chip-8");
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(64 * SCALE, 32 * SCALE)) {};

    sf::Uint8* pixels = new sf::Uint8[64 * 32 * 4];
    sf::Texture texture;
    texture.create(64, 32);
    sf::Sprite sprite(texture);

    //Sets emulator to run at 60hz
    window.setFramerateLimit(60);

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
            pixels[i] = (bit8)cpu->video[i / 4] ? 120 : 33;//red;
            pixels[i + 1] = (bit8)cpu->video[i/4] ? 200 : 33; //green;
            pixels[i + 2] = (bit8)cpu->video[i / 4] ? 120 : 150;//blue;
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