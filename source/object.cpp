#include "object.h"

Object::Object(int x, int y, int scaleFactor, SDL_Renderer* renderer) : x(x), y(y), scaleFactor(scaleFactor) {
    SDL_Surface* surface = IMG_Load("assets/pineTree.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

int Object::draw(SDL_Renderer* renderer, const SDL_Point& player, const SDL_Point& screenCenter) {
    SDL_Rect dstRect = {screenCenter.x + x - player.x, screenCenter.y + y - player.y, width * scaleFactor, height * scaleFactor};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect r = adjustRectForScreen(collistionRect, player, screenCenter);
    SDL_RenderDrawRect(renderer, &r);

    return 0;
}

PineTree::PineTree(int x, int y, int scaleFactor, SDL_Renderer* renderer) : Object(x, y, scaleFactor, renderer) {
    pathToTexture = "assets/pineTree.png";
    width = 29; 
    height = 68;
    collistionRect = {x + 8 * scaleFactor, y + 62 * scaleFactor, 14 * scaleFactor, 6 * scaleFactor};
}