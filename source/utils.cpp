#include "utils.h"

SDL_Rect adjustRectForScreen(const SDL_Rect& rect, const SDL_Point& playerCoordinates, const SDL_Point& screenCenter) {
    return SDL_Rect{rect.x - playerCoordinates.x + screenCenter.x, rect.y - playerCoordinates.y + screenCenter.y, rect.w, rect.h};
}