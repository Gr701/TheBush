#include <SDL.h>
#include <random>
#include <time.h>

#include "chunk.h"
#include "object.h"
#include "quickSort.h"

Chunk::Chunk() {}

Chunk::Chunk(int chunkX, int chunkY, int chunkSide, int index, SDL_Renderer* renderer) 
: chunkX(chunkX), chunkY(chunkY), chunkSide(chunkSide), index(index)
{
    srand(time(NULL) + index * 20);

    if (chunkType == "forest") {
        int treesNumber = 10;
        int yList[treesNumber];
        for (int i = 0; i < treesNumber; i++) {
            yList[i] = rand() % chunkSide + chunkSide * chunkY;
        }
        quickSort(&yList[0], 0, treesNumber - 1);

        for (int i = 0; i < treesNumber; i++) {
            int x = rand() % chunkSide + chunkSide * chunkX;
            objects.push_back(PineTree(x, yList[i], 4, renderer)); 
        }
        //objects.push_back(PineTree(0 + chunkSide * chunkX, 100, 4, renderer));
        //objects.push_back(PineTree(chunkSide - 1 + chunkSide * chunkX, 120, 4, renderer));
  
        objects.shrink_to_fit();
    }
    
}

int Chunk::draw(SDL_Renderer* renderer, SDL_Point* player, SDL_Point* screenCenter) {
    SDL_Rect rect = {chunkX * chunkSide - player->x + screenCenter->x, chunkY * chunkSide - player->y + screenCenter->y, chunkSide, chunkSide};
    SDL_RenderDrawRect(renderer, &rect);
    return 0;
}
