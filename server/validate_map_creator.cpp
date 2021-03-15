#include "worldmap.hpp"

using namespace Netocracy;

bool defineRiverRect(SDL_Rect &riverRect, SDL_Rect centerRiverRect, SDL_Rect landRect, const char* direction) {
	if(!strcmp(direction, "N-S") || !strcmp(direction, "S-N")) {
		riverRect.w = centerRiverRect.w;
		riverRect.h = centerRiverRect.h * 2;
		riverRect.x = centerRiverRect.x;
		if(!strcmp(direction, "N-S")) riverRect.y = centerRiverRect.y - centerRiverRect.h;
		else riverRect.y = centerRiverRect.y + centerRiverRect.h;
		return true;
	}
	else if(!strcmp(direction, "E-W") || !strcmp(direction, "W-E")) {
		riverRect.h = centerRiverRect.h;
		riverRect.w = centerRiverRect.w * 2;
		riverRect.y = centerRiverRect.y;
		if(!strcmp(direction, "E-W")) riverRect.x = centerRiverRect.x - centerRiverRect.w;
		else riverRect.x = centerRiverRect.x + centerRiverRect.w;
		return true;
	}
	return false;
}

void getRiverRects(std::vector<SDL_Rect>& riverRects, SDL_Rect landRect, Netocracy::MapSquare* landSquare) {
	SDL_Rect centerRiverRect;
	SDL_Rect fromRiverRect;
	SDL_Rect toRiverRect;
	centerRiverRect.h = int(landRect.h / 5.0);
	centerRiverRect.w = int(landRect.w / 5.0);
	centerRiverRect.x = landRect.x + int(landRect.w * 2.0 / 5.0);
	centerRiverRect.y = landRect.y + int(landRect.h * 2.0 / 5.0);
	riverRects.push_back(centerRiverRect);
	if (defineRiverRect(fromRiverRect, centerRiverRect, landRect, landSquare->getRiverDirectionStr(true)))
		riverRects.push_back(fromRiverRect);
	if(defineRiverRect(toRiverRect, centerRiverRect, landRect, landSquare->getRiverDirectionStr(false)))
		riverRects.push_back(toRiverRect);
}

void drawMap(Netocracy::Map map, SDL_Surface* screenSurface, float maxElevation) {
	for(int xi=0; xi<MAP_SIZE; ++xi) {
		for(int yi=0; yi<MAP_SIZE; ++yi) {
			Netocracy::MapSquare* square = map.getSquare(xi, yi);
			float elevation = square->getElevation();
			SDL_Rect rect; rect.x = xi * SQUARE_SIZE; rect.y = yi * SQUARE_SIZE; rect.w = SQUARE_SIZE; rect.h = SQUARE_SIZE;
			if(elevation >= 0.0) {
				int brightness = int(elevation * 255.0 / maxElevation);
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, brightness, 0x60, 0x00));
			}
			else {
				SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x80));
			}
			if(square->getHaveRiver()) {
				std::vector<SDL_Rect> riverRects;
				getRiverRects(riverRects, rect, square);
				for(auto riverRect : riverRects) {
					SDL_FillRect(screenSurface, &riverRect, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x80));
				}
			}
		}
	}
}

int main(int argc, char* args[]) {
	Netocracy::MapBuilder builder;
	Netocracy::Map newMap = builder.generateRandomMap(19870911, MAP_SIZE, MAP_SIZE, 10000.0);
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