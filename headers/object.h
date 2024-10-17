#pragma once

#include <SDL.h>

class Object
{
    public:
        Object();
        Object(int x, int y, int scaleFactor, SDL_Renderer* renderer);

        int draw(SDL_Renderer* renderer, SDL_Point* player, SDL_Point* screenCenter);
        
        SDL_Texture* texture;

        int x, y; //x and y in the world
        int width = 29, height = 68; 
        const char* pathToTexture;
        int scaleFactor;
};

class PineTree : public Object
{
    using Object::Object;

    const char* pathToTexture = "assets/pineTree.png";
    int width = 29; 
    int height = 68;
};
