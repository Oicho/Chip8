#include <SDL.h>
#include "Graphics.h"
#include "Chip.h"

int parseInput(SDL_Event event) {
	switch (event.key.keysym.sym) {
	case SDLK_1:
		return 0;
		break;
	case SDLK_2:
		return 1;
		break;
	case SDLK_3:
		return 2;
		break;
	case SDLK_4:
		return 3;
		break;
	case SDLK_q:
		return 4;
		break;
	case SDLK_w:
		return 5;
		break;
	case SDLK_e:
		return 6;
		break;
	case SDLK_r:
		return 7;
		break;
	case SDLK_a:
		return 8;
		break;
	case SDLK_s:
		return 9;
		break;
	case SDLK_d:
		return 0xA;
		break;
	case SDLK_f:
		return 0xB;
		break;
	case SDLK_z:
		return 0xC;
		break;
	case SDLK_x:
		return 0xD;
		break;
	case SDLK_c:
		return 0xE;
		break;
	case SDLK_v:
		return 0xF;
		break;
	}
}

int waitForInput() {
	//TODO this is not working 
	SDL_Event event;
	bool isKeydown = false;
	while (!isKeydown) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN)
			isKeydown = true;
	}
	return parseInput(event);
}

void drawScreen(std::vector<std::vector<bool>> screen, SDL_Surface* screenSurface) {
	SDL_Rect position{ 0, 0, PIXEL_SIZE, PIXEL_SIZE };
	for (size_t i = 0; i < screen.size(); i++) {
		for (size_t j = 0; j < screen[i].size(); j++) {
			if (screen[i][j]) {
				position.x = PIXEL_SIZE * i;
				position.y = PIXEL_SIZE * j;

				SDL_FillRect(screenSurface, &position, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			}
		}
	}
}