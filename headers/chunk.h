#pragma once

#include <SDL.h>
#include <vector> 

#include "object.h"

class Chunk
{
    public:
        Chunk();

        Chunk(int chunkX, int chunkY, int chunkSide, int index, SDL_Renderer* renderer);

        int draw(SDL_Renderer* renderer, SDL_Point* player, SDL_Point* screenCenter);
        int drawNeighborChunks(SDL_Renderer* renderer, SDL_Point* player, SDL_Point* screenCenter);

        int chunkSide;
        int chunkX, chunkY; //coordinate of chunk (1 steep - 1 chunk)
        int index;
        int neighborChunks[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1}; //like a table with 0 0 at top left // addresses in the vector
        bool isFilledNeighbors = false;
        std::vector<Object> objects;
    private:
        const char* chunkType = "forest";
        
};