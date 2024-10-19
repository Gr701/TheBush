#include <SDL.h>

#include "game.h"


struct Vector2 {int x; int y;};

int main (int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0) {
        SDL_Log("Initialize error");
    }
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    //is SDL_WINDOW_ALLOW_HIGHDPI neccesary???
    SDL_Window* window =  SDL_CreateWindow("Bush walking", -1910, 40, 1900, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        SDL_Log("SDL_GL_CreateContext error: %s", SDL_GetError());
    }
    if (SDL_GL_SetSwapInterval(1) != 0) {
        SDL_Log("SDL_GL_SetSwapInteval error: %s", SDL_GetError());
    } 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    

    Game game = Game(renderer, 60);
    game.start();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
