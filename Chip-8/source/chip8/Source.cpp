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
#include <SFML/Audio.hpp>
#include <vector>

#define SCALE 30

short SineWave(double time, double freq, double amp)
{
    short result;
    double tpc = 44100 / freq;
    double cycles = time / tpc;
    double rad = 6.28318507 * cycles;
    short amplitude = 32767 * amp;
    result = amplitude * sin(rad);
    return result;
}

int main()
{
    CPU* cpu = new CPU();
    cpu->Initialize();
    cpu->LoadROM("C:\\Users\\slayd\\source\\repos\\Miosyo\\Emulators\\Chip-8\\roms\\games\\Breakout [Carmelo Cortez, 1979].ch8");

    sf::RenderWindow window(sf::VideoMode(64 * SCALE, 32 * SCALE), "Chip-8");

    //Load render target and texture
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(64 * SCALE, 32 * SCALE)) {};
    sf::Uint8* pixels = new sf::Uint8[64 * 32 * 4];
    sf::Texture t;
    t.create(64, 32);
    sf::Sprite s(t);

    //Load sound
    sf::SoundBuffer buffer;
    std::vector<sf::Int16> samples;
    for (int i = 0; i < 44100; i++)
    {
        samples.push_back(SineWave(i, 720, 0.7));
    }
    buffer.loadFromSamples(&samples[0], samples.size(), 1, 44100);

    sf::Sound sound;
    sound.setLoop(true);
    sound.setBuffer(buffer);

    //Sets emulator to run at 60hz
    window.setFramerateLimit(60);

    const sf::Texture& texture = renderTexture.getTexture();
    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Num1:
                {
                    cpu->keypad[1] = true;
                    break;
                }
                case sf::Keyboard::Key::Num2:
                {
                    cpu->keypad[2] = true;
                    break;
                }
                case sf::Keyboard::Key::Num3:
                {
                    cpu->keypad[3] = true;
                    break;
                }
                case sf::Keyboard::Key::Num4:
                {
                    cpu->keypad[12] = true;
                    break;
                }
                case sf::Keyboard::Key::Q:
                {
                    cpu->keypad[4] = true;
                    break;
                }
                case sf::Keyboard::Key::W:
                {
                    cpu->keypad[5] = true;
                    break;
                }
                case sf::Keyboard::Key::E:
                {
                    cpu->keypad[6] = true;
                    break;
                }
                case sf::Keyboard::Key::R:
                {
                    cpu->keypad[13] = true;
                    break;
                }
                case sf::Keyboard::Key::A:
                {
                    cpu->keypad[7] = true;
                    break;
                }
                case sf::Keyboard::Key::S:
                {
                    cpu->keypad[8] = true;
                    break;
                }
                case sf::Keyboard::Key::D:
                {
                    cpu->keypad[9] = true;
                    break;
                }
                case sf::Keyboard::Key::F:
                {
                    cpu->keypad[14] = true;
                    break;
                }
                case sf::Keyboard::Key::Z:
                {
                    cpu->keypad[10] = true;
                    break;
                }
                case sf::Keyboard::Key::X:
                {
                    cpu->keypad[0] = true;
                    break;
                }
                case sf::Keyboard::Key::C:
                {
                    cpu->keypad[11] = true;
                    break;
                }
                case sf::Keyboard::Key::V:
                {
                    cpu->keypad[15] = true;
                    break;
                }
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Num1:
                {
                    cpu->keypad[1] = false;
                    break;
                }
                case sf::Keyboard::Key::Num2:
                {
                    cpu->keypad[2] = false;
                    break;
                }
                case sf::Keyboard::Key::Num3:
                {
                    cpu->keypad[3] = false;
                    break;
                }
                case sf::Keyboard::Key::Num4:
                {
                    cpu->keypad[12] = false;
                    break;
                }
                case sf::Keyboard::Key::Q:
                {
                    cpu->keypad[4] = false;
                    break;
                }
                case sf::Keyboard::Key::W:
                {
                    cpu->keypad[5] = false;
                    break;
                }
                case sf::Keyboard::Key::E:
                {
                    cpu->keypad[6] = false;
                    break;
                }
                case sf::Keyboard::Key::R:
                {
                    cpu->keypad[13] = false;
                    break;
                }
                case sf::Keyboard::Key::A:
                {
                    cpu->keypad[7] = false;
                    break;
                }
                case sf::Keyboard::Key::S:
                {
                    cpu->keypad[8] = false;
                    break;
                }
                case sf::Keyboard::Key::D:
                {
                    cpu->keypad[9] = false;
                    break;
                }
                case sf::Keyboard::Key::F:
                {
                    cpu->keypad[14] = false;
                    break;
                }
                case sf::Keyboard::Key::Z:
                {
                    cpu->keypad[10] = false;
                    break;
                }
                case sf::Keyboard::Key::X:
                {
                    cpu->keypad[0] = false;
                    break;
                }
                case sf::Keyboard::Key::C:
                {
                    cpu->keypad[11] = false;
                    break;
                }
                case sf::Keyboard::Key::V:
                {
                    cpu->keypad[15] = false;
                    break;
                }
                }
            }
        }

        cpu->Cycle();
        if (cpu->sound != 0) sound.play();
        else sound.stop();

        if (cpu->draw)
        {
            for (int i = 0; i < 64 * 32 * 4; i += 4)
            {
                pixels[i] = (bit8)cpu->video[i / 4] ? 120 : 33;//red;
                pixels[i + 1] = (bit8)cpu->video[i / 4] ? 200 : 33; //green;
                pixels[i + 2] = (bit8)cpu->video[i / 4] ? 120 : 150;//blue;
                pixels[i + 3] = 255;//alpha;
            }
            t.update(pixels);

            //renderTexture.clear();
            //Draw
            renderTexture.draw(s);
            renderTexture.display();

            cpu->draw = false;
        }

        sprite.setScale(sf::Vector2f(SCALE, SCALE));
        window.draw(sprite);
        window.display();
    }

    return 0;
}