#pragma once

#include <SDL.h>

SDL_Rect adjustRectForScreen(const SDL_Rect& rect, const SDL_Point& playerCoordinates, const SDL_Point& screenCenter);
