#ifndef GRAPHICS
#define GRAPHICS
#include <SDL.h>
#include <vector>

int waitForInput();
void drawScreen(std::vector<std::vector<bool>> screen, SDL_Surface* screenSurface);
const int PIXEL_SIZE = 10;
#endif