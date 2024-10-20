#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <utils.h>

class Object {
    public:
        Object(int x, int y, int scaleFactor, SDL_Renderer* renderer); 
        int draw(SDL_Renderer* renderer, const SDL_Point& player, const SDL_Point& screenCenter);

        SDL_Texture* texture;
        int x, y; //x and y in the world
        int width, height; 
        SDL_Rect collistionRect;
        const char* pathToTexture;
        int scaleFactor;
};

class PineTree : public Object {
    /* using Object::Object; */
    public: 
        PineTree(int x, int y, int scaleFactor, SDL_Renderer* renderer);
};
