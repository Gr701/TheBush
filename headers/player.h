#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <utils.h>

class Player
{
    public: 
        SDL_Point coordinates; //coordinates of center of the screen in the worlds

        Player();

        Player(int x, int y, int scaleFactor, const SDL_Point& screenCenter, SDL_Renderer* renderer);

        int move();

        int draw(SDL_Renderer* renderer);
        
        SDL_Rect collisionRect;

    private:
        
        int width = 16;
        int height = 17;
        int prevX, prevY;
        int speed = 5;
        const Uint8 *kState;

        bool isRight, isStay;

        SDL_Texture* texture;

        SDL_Rect idleSrcRect;
        SDL_Rect dstRect;
        
};