#include "CPU.h"
#include "Exceptions.h"
#include "SDL.h"
#include <iostream>

void run()
{}

int main(int argc, char* argv[])
{
	try {
		CPU cpu = CPU("");
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
		bool running = true;
		while (running)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event) != 0)
			{
				switch (event.type)
				{
				case SDL_QUIT:
					running = false;
					break;
				}
			}
			cpu.cycle();
			SDL_RenderClear(renderer);
			SDL_UpdateTexture(texture, NULL, cpu.getVideoMemory(), 4 * 64);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	catch (FileNotFoundException exception)
	{
		std::cerr << exception.what() << "\nFile: " << exception.getFilename() << "\n";
	}
	catch (RomTooLargeException exception)
	{
		std::cerr << exception.what() << "\n";
	}
	return 0;
}
