#include "player.h"

Player::Player() {}

Player::Player(int x, int y, int scaleFactor, const SDL_Point& screenCenter, SDL_Renderer* renderer) 
: coordinates({x, y}) 
{
    SDL_Surface* surface = IMG_Load("assets/bush.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
	idleSrcRect = {0, 0, 16, 17};
    dstRect = {screenCenter.x, screenCenter.y, 16 * scaleFactor, 17 * scaleFactor};
	collisionRect = {coordinates.x + 2 * scaleFactor, coordinates.y + 6 * scaleFactor, 12 * scaleFactor, 10 * scaleFactor};
}

int Player::move() {
	prevX = coordinates.x;
	prevY = coordinates.y;

	SDL_PumpEvents();
	kState = SDL_GetKeyboardState(NULL);
	if (kState[SDL_SCANCODE_W]) {
		coordinates.y -= speed;
		collisionRect.y -= speed;
	}
	if (kState[SDL_SCANCODE_A]) {
		coordinates.x -= speed;
		collisionRect.x -= speed;
	}
	if (kState[SDL_SCANCODE_S]) {
		coordinates.y += speed;
		collisionRect.y += speed;
	}
	if (kState[SDL_SCANCODE_D]) {
		coordinates.x += speed;
		collisionRect.x += speed;
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

	SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
	int screenWidth, screenHeight; 
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    SDL_Point screenCenter = {screenWidth/2, screenHeight/2}; 
	SDL_Rect r = adjustRectForScreen(collisionRect, coordinates, screenCenter);
	SDL_RenderDrawRect(renderer, &r);
    return 0;
}