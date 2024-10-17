#include <SDL.h>

#include "game.h"


struct Vector2 {int x; int y;};

int main (int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0) {SDL_Log("Initialize error");}
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Window* window =  SDL_CreateWindow("Bush walking", 10, 40, 1900, 1000, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    Game game = Game(renderer, 60);
    game.start();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
