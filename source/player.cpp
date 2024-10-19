#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "player.h"

Player::Player() {}

Player::Player(int x, int y, int scaleFactor, SDL_Point* screenCenter, SDL_Renderer* renderer) 
: coordinates({x, y}) 
{
    SDL_Surface* surface = IMG_Load("assets/bush.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
 
    dstRect = {screenCenter->x, screenCenter->y, 16 * scaleFactor, 17 * scaleFactor};
}

int Player::move() {
	prevX = coordinates.x;
	prevY = coordinates.y;

	SDL_PumpEvents();
	kState = SDL_GetKeyboardState(NULL);
	if (kState[SDL_SCANCODE_W]) {
		coordinates.y -= speed;
	}
	if (kState[SDL_SCANCODE_A]) {
		coordinates.x -= speed;
	}
	if (kState[SDL_SCANCODE_S]) {
		coordinates.y += speed;
	}
	if (kState[SDL_SCANCODE_D]) {
		coordinates.x += speed;
	}
	
	if (prevX == coordinates.x && prevY == coordinates.y) {
		isStay = true;
	} else if (prevX > coordinates.x) {
		isRight = false; isStay = false;
	} else if (prevX < coordinates.x) {
		isRight = true; isStay = false;
	} else {
		isStay = false;
	}
	return 0;
}

int Player::draw(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &idleSrcRect, &dstRect);
    return 0;
}