
//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include "Chip.h"
#include "Graphics.h"
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	SDL_Event event;
	bool quit = false;
	Chip c;
	c.loadROM("ROM\\TICTAC");
	std::cout << sizeof(unsigned short) << std::endl;
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cerr <<"Window could not be created! SDL_Error: " << SDL_GetError() <<std::endl;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
			while (!quit) {
				c.cycle();
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
				drawScreen(c.getScreen(), screenSurface);

				SDL_UpdateWindowSurface(window);

				while (SDL_PollEvent(&event) != 0)
				{
					//User requests quit
					if (event.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				
				//Update the surface
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}