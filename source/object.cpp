#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

#include "object.h"

Object::Object(int x, int y, int scaleFactor, SDL_Renderer* renderer) 
: x(x), y(y), scaleFactor(scaleFactor)
{
    //cout << " x = " << x << " y = " << y << endl;
    SDL_Surface* surface = IMG_Load("assets/pineTree.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

int Object::draw(SDL_Renderer* renderer, SDL_Point* player, SDL_Point* screenCenter)
{
    SDL_Rect dstRect = {screenCenter->x + x - player->x, screenCenter->y + y - player->y, width * scaleFactor, height * scaleFactor};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    return 0;
}