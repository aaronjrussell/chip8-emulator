#include "CPU.h"
#include "Exceptions.h"
#include "SDL.h"
#include <chrono>
#include <iostream>

int main(int argc, char* argv[])
{
	const int cycleDelay = 4;
	try {
		CPU cpu = CPU("");
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
		std::chrono::steady_clock::time_point lastCycleTime = std::chrono::high_resolution_clock::now();
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
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_1:
						cpu.setInput(1, true);
						break;
					case SDLK_2:
						cpu.setInput(2, true);
						break;
					case SDLK_3:
						cpu.setInput(3, true);
						break;
					case SDLK_4:
						cpu.setInput(12, true);
						break;
					case SDLK_q:
						cpu.setInput(4, true);
						break;
					case SDLK_w:
						cpu.setInput(5, true);
						break;
					case SDLK_e:
						cpu.setInput(6, true);
						break;
					case SDLK_r:
						cpu.setInput(13, true);
						break;
					case SDLK_a:
						cpu.setInput(7, true);
						break;
					case SDLK_s:
						cpu.setInput(8, true);
						break;
					case SDLK_d:
						cpu.setInput(9, true);
						break;
					case SDLK_f:
						cpu.setInput(14, true);
						break;
					case SDLK_z:
						cpu.setInput(10, true);
						break;
					case SDLK_x:
						cpu.setInput(0, true);
						break;
					case SDLK_c:
						cpu.setInput(11, true);
						break;
					case SDLK_v:
						cpu.setInput(15, true);
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
					case SDLK_1:
						cpu.setInput(1, false);
						break;
					case SDLK_2:
						cpu.setInput(2, false);
						break;
					case SDLK_3:
						cpu.setInput(3, false);
						break;
					case SDLK_4:
						cpu.setInput(12, false);
						break;
					case SDLK_q:
						cpu.setInput(4, false);
						break;
					case SDLK_w:
						cpu.setInput(5, false);
						break;
					case SDLK_e:
						cpu.setInput(6, false);
						break;
					case SDLK_r:
						cpu.setInput(13, false);
						break;
					case SDLK_a:
						cpu.setInput(7, false);
						break;
					case SDLK_s:
						cpu.setInput(8, false);
						break;
					case SDLK_d:
						cpu.setInput(9, false);
						break;
					case SDLK_f:
						cpu.setInput(14, false);
						break;
					case SDLK_z:
						cpu.setInput(10, false);
						break;
					case SDLK_x:
						cpu.setInput(0, false);
						break;
					case SDLK_c:
						cpu.setInput(11, false);
						break;
					case SDLK_v:
						cpu.setInput(15, false);
						break;
					}
					break;
				}
			}
			std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
			float delta = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
			if (delta > cycleDelay)
			{
				lastCycleTime = currentTime;
				cpu.cycle();
				SDL_RenderClear(renderer);
				SDL_UpdateTexture(texture, NULL, cpu.getVideoMemory(), 4 * 64);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
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
