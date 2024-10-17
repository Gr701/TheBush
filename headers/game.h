#pragma once 

#include <SDL.h>
#include <vector>

#include "player.h"
#include "chunk.h"

class Game
{
    public:
        Game(SDL_Renderer* renderer, int FPS);
        int start();

    private: 
        SDL_Renderer* renderer;
        SDL_Event event;
        
        int frameDelay; //FPS
        
        int scaleFactor = 4;

        Player player;

        std::vector<Chunk> chunks;
        int currentChunkIndex; //in the chunks vector
        const int chunkSide = 1000;

        SDL_Point screenCenter;

        int draw();

        int fillNeighborChunks(Chunk* chunk);
        
};
