#include "worldmap.hpp"
#include <SDL2/SDL.h>
#include <time.h>

using namespace Netocracy;

SDL_Rect* getRiverRect(SDL_Rect landRect, Netocracy::WorldMapSquare* landSquare) {
	Direction* riverDirection = landSquare->getRiverDirection();
	char directionStr[4];
	riverDirection->getDirectionStr(directionStr);
	SDL_Rect* riverRect = new SDL_Rect();
	if(!strcmp(directionStr, "N-S") or !strcmp(directionStr, "S-N")) {
		riverRect->h = landRect.h;
		riverRect->w = int(landRect.w / 5.0);
		riverRect->x = landRect.x + int(landRect.w * 2.0 / 5.0);
		riverRect->y = landRect.y;
	}
	else {
		riverRect->w = landRect.w;
		riverRect->h = int(landRect.h / 5.0);
		riverRect->y = landRect.y + int(landRect.h * 2.0 / 5.0);
		riverRect->x = landRect.x;
	}
	return riverRect;
}

void drawMap(Netocracy::WorldMap map, SDL_Surface* screenSurface, float maxElevation) {
	for(int xi=0; xi<MAP_SIZE; ++xi) {
		for(int yi=0; yi<MAP_SIZE; ++yi) {
			Netocracy::WorldMapSquare* square = map.getSquare(xi, yi);
			float elevation = square->getElevation();
			SDL_Rect rect; rect.x = xi * SQUARE_SIZE; rect.y = yi * SQUARE_SIZE; rect.w = SQUARE_SIZE; rect.h = SQUARE_SIZE;
			if(elevation >= 0.0) {
				int brightness = int(elevation * 255.0 / maxElevation);
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, brightness, 0x60, 0x00));
			}
			else {
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x80));
			}
			if(square->haveRiver()) {
				SDL_Rect* riverRect = getRiverRect(rect, square);
				SDL_FillRect(screenSurface, riverRect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x80));
			}
		}
	}
}

int main(int argc, char* args[]) {
	Netocracy::MapBuilder builder;
	// Netocracy::WorldMap newMap = builder.generateRandomMap(19870911, MAP_SIZE, MAP_SIZE, 10000.0);
	Netocracy::WorldMap newMap = builder.generateRandomMap(time(NULL), MAP_SIZE, MAP_SIZE, 10000.0);
	float maxElevation = builder.getMaxElevation();
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "could not initialize sdl2: " << SDL_GetError() << '\n';
		return 1;
	}
	window = SDL_CreateWindow(
					"hello_sdl2",
					SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					SQUARE_SIZE * MAP_SIZE, SQUARE_SIZE * MAP_SIZE,
					SDL_WINDOW_SHOWN
					);
	if (window == NULL) {
		std::cout << "could not create window: " << SDL_GetError() << '\n';
		return 1;
	}
	screenSurface = SDL_GetWindowSurface(window);
	drawMap(newMap, screenSurface, maxElevation);
	SDL_UpdateWindowSurface(window);
	SDL_Event e;
	while(true) {
		SDL_PollEvent(&e);
		if(e.type == SDL_QUIT) break;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}