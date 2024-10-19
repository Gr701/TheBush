#include <iostream>
#include <cmath>
using namespace std;

#include "game.h"   

struct Timer ///////////////////
{
    Uint64 previous_ticks{};
    float elapsed_seconds{};

    void tick()
    {
        const Uint64 current_ticks{ SDL_GetPerformanceCounter() };
        const Uint64 delta{ current_ticks - previous_ticks };
        previous_ticks = current_ticks;
        static const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
        elapsed_seconds = delta / static_cast<float>(TICKS_PER_SECOND);
    }
};

Game::Game(SDL_Renderer* renderer, int FPS) : renderer(renderer) 
{
    frameDelay = 1000/FPS;

    int screenWidth, screenHeight; 
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    screenCenter = {screenWidth/2, screenHeight/2}; 

    player = Player(0, 0, scaleFactor, &screenCenter, renderer);
    chunks.push_back(Chunk(0, 0, chunkSide, 0, renderer));
    currentChunkIndex = 0;
    fillNeighborChunks(&chunks[0]);
}

int Game::start() {
    bool quit = false;
    while (!quit) {   
        //MOVE 
        player.move();
        //

        //LOGIC
        //if change chunk
        int chunkXDifference = player.coordinates.x / chunkSide - chunks[currentChunkIndex].chunkX;
        int chunkYDifference = player.coordinates.y / chunkSide - chunks[currentChunkIndex].chunkY;
        if (player.coordinates.x < 0) chunkXDifference--;
        if (player.coordinates.y < 0) chunkYDifference--;

        if (chunkXDifference != 0 || chunkYDifference != 0)
        {
            chunkXDifference++;
            chunkYDifference++;
            currentChunkIndex = chunks[currentChunkIndex].neighborChunks[3 * chunkYDifference + chunkXDifference];
            if (!chunks[currentChunkIndex].isFilledNeighbors)
            {
                fillNeighborChunks(&chunks[currentChunkIndex]);
            }
        }
        //

        //INPUT HANDLING
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        //

        //DRAW
        draw();
        //
    }
    return 0;
}

int testX = 0;
int Game::draw() {
    SDL_SetRenderDrawColor(renderer, 64, 125, 91, 255); 
    SDL_RenderClear(renderer);
    //loaded chunks
    for (int chunkLine = 0; chunkLine < 3; chunkLine++) {
        Chunk* pChunk0 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 0]]; //pointers to chunks from neigbor array of current chunk
        Chunk* pChunk1 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 1]]; // these three chunks are in one horizontal line 
        Chunk* pChunk2 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 2]];
        
        SDL_SetRenderDrawColor(renderer, (chunkLine + 1) * 80, 30, 190, 255);
        pChunk0->draw(renderer, &player.coordinates, &screenCenter);
        pChunk1->draw(renderer, &player.coordinates, &screenCenter);
        pChunk2->draw(renderer, &player.coordinates, &screenCenter);
        
        int i = 0, j = 0, k = 0;
        while (i < pChunk0->objects.size() || j < pChunk1->objects.size() || k < pChunk2->objects.size()) {
            int tree0Y, tree1Y, tree2Y; 
            if (i == pChunk0->objects.size()) { 
                tree0Y = (pChunk0->chunkY + 2) * chunkSide; 
            } else {
                tree0Y = pChunk0->objects[i].y;
            }
            if (j == pChunk1->objects.size()) {
                tree1Y = (pChunk1->chunkY + 2) * chunkSide;
            } else {
                tree1Y = pChunk1->objects[j].y;
            }
            if (k == pChunk2->objects.size()) {
                tree2Y = (pChunk2->chunkY + 2) * chunkSide;
            } else {
                tree2Y = pChunk2->objects[k].y;
            }

            if (tree0Y <= tree1Y && tree0Y <= tree2Y) {
                pChunk0->objects[i].draw(renderer, &player.coordinates, &screenCenter);
                i++;
            } else if (tree1Y <= tree0Y && tree1Y <= tree2Y) {
                pChunk1->objects[j].draw(renderer, &player.coordinates, &screenCenter);
                j++;
            } else {
                pChunk2->objects[k].draw(renderer, &player.coordinates, &screenCenter);
                k++;
            }
        }
    }
    player.draw(renderer);
    SDL_RenderPresent(renderer); 
    return 0;
}

int Game::fillNeighborChunks(Chunk* pChunk) {
    int currentChunkX = pChunk->chunkX;
    int currentChunkY = pChunk->chunkY;
    //existing chunks
    for (int i = 0; i < chunks.size(); i++) {
        int chunkXDifference = chunks[i].chunkX - currentChunkX;
        int chunkYDifference = chunks[i].chunkY - currentChunkY;
        if (-1 <= chunkXDifference && chunkXDifference <= 1 && -1 <= chunkYDifference && chunkYDifference <= 1) {
            chunkXDifference++;
            chunkYDifference++;
            chunks[currentChunkIndex].neighborChunks[chunkYDifference * 3 + chunkXDifference] = i;
        }
    }
    //creating new chunks
    for (int i = 0; i < 9; i++) {
        if (chunks[currentChunkIndex].neighborChunks[i] == -1) {
            int indexOfNewChunk = chunks.size();
            chunks[currentChunkIndex].neighborChunks[i] = indexOfNewChunk;
            chunks.push_back(Chunk(i % 3 - 1 + currentChunkX, i / 3 - 1 + currentChunkY, chunkSide, indexOfNewChunk, renderer)); 
        }
    }
    chunks[currentChunkIndex].isFilledNeighbors = true;
    return 0;
}