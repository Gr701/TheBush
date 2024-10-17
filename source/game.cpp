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

int Game::start()
{

    // Timing constants
    const int UPDATE_FREQUENCY{ 60 };
    const float CYCLE_TIME{ 1.0f / UPDATE_FREQUENCY };
    // System timing
    static Timer system_timer;
    float accumulated_seconds{ 0.0f };

    bool quit = false;
    while (!quit)
    {

        // Update clock
        system_timer.tick();
        accumulated_seconds += system_timer.elapsed_seconds;

        Uint32 frameStart = SDL_GetTicks(); //FPS

        //if (isgreater(accumulated_seconds, CYCLE_TIME)) ///////////
        {
            accumulated_seconds = 0; ////////////////////

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

            //DRAW
            draw();
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

            //FPS
            int frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
                int testTime = SDL_GetTicks() - frameStart;
                if (testTime > 17) cout << testTime << endl;

                ///////////
                
            }
            else
            {
                cout << "over delay" << endl;
            }
            //
        }
    }

    return 0;
}

int testX = 0;
int Game::draw()
{
    SDL_SetRenderDrawColor(renderer, 64, 125, 91, 255); 
    SDL_RenderClear(renderer);
    
    //all chunks
    //SDL_SetRenderDrawColor(renderer, 190, 80, 80, 255);
    //for (int i = 0; i < chunks.size(); i++)
    //{
    //    chunks[i].draw(renderer, &player.coordinates, &screenCenter);
    //} 
    //loaded chunks
    //SDL_SetRenderDrawColor(renderer, 80, 80, 190, 255);
    //for (int i = 0; i < 9; i++)
    //{
    //    if (i == 4) continue;
    //    chunks[chunks[currentChunkIndex].neighborChunks[i]].draw(renderer, &player.coordinates, &screenCenter);
    //}
    //current chunk
    //SDL_SetRenderDrawColor(renderer, 80, 255, 190, 255);
    //chunks[currentChunkIndex].draw(renderer, &player.coordinates, &screenCenter);

    testX += 5;
    SDL_SetRenderDrawColor(renderer, 255, 30, 190, 255);
    SDL_Rect testRect = {testX, 200, 50, 50};
    SDL_RenderFillRect(renderer, &testRect);

    //loaded chunks
    for (int chunkLine = 0; chunkLine < 3; chunkLine++)
    {
        Chunk* pChunk0 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 0]]; //pointers to chunks from neigbor array of current chunk
        Chunk* pChunk1 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 1]]; // these three chunks are in one horizontal line 
        Chunk* pChunk2 = &chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 2]];
        
        SDL_SetRenderDrawColor(renderer, (chunkLine + 1) * 80, 30, 190, 255);
        pChunk0->draw(renderer, &player.coordinates, &screenCenter);
        pChunk1->draw(renderer, &player.coordinates, &screenCenter);
        pChunk2->draw(renderer, &player.coordinates, &screenCenter);

        int i = 0, j = 0, k = 0;
        while (i < pChunk0->objects.size() || j < pChunk1->objects.size() || k < pChunk2->objects.size())
        {
            int tree0Y, tree1Y, tree2Y; 
            if (i == pChunk0->objects.size()) tree0Y = (pChunk0->chunkY + 2) * chunkSide; 
            else tree0Y = pChunk0->objects[i].y;
            if (j == pChunk1->objects.size()) tree1Y = (pChunk1->chunkY + 2) * chunkSide; 
            else tree1Y = pChunk1->objects[j].y;
            if (k == pChunk2->objects.size()) tree2Y = (pChunk2->chunkY + 2) * chunkSide; 
            else tree2Y = pChunk2->objects[k].y;

            if (tree0Y <= tree1Y && tree0Y <= tree2Y)
            {
                pChunk0->objects[i].draw(renderer, &player.coordinates, &screenCenter);
                i++;
            }
            else if (tree1Y <= tree0Y && tree1Y <= tree2Y)
            {
                pChunk1->objects[j].draw(renderer, &player.coordinates, &screenCenter);
                j++;
            }
            else 
            {
                pChunk2->objects[k].draw(renderer, &player.coordinates, &screenCenter);
                k++;
            }
        }
    }
    
    player.draw(renderer);

    SDL_RenderPresent(renderer); 
    return 0;
}

int Game::fillNeighborChunks(Chunk* pChunk)
{
    int currentChunkX = pChunk->chunkX;
    int currentChunkY = pChunk->chunkY;
    //cout << "---------------" << endl;
    //cout << "current chunk index = " << currentChunkIndex << endl;
    //existing chunks
    for (int i = 0; i < chunks.size(); i++)
    {
        int chunkXDifference = chunks[i].chunkX - currentChunkX;
        int chunkYDifference = chunks[i].chunkY - currentChunkY;
        
        //cout << "chunks[i].index = " << chunks[i].index << endl;
        //cout << chunkXDifference << "  " << chunkYDifference << endl;
        if (-1 <= chunkXDifference && chunkXDifference <= 1 && -1 <= chunkYDifference && chunkYDifference <= 1) 
        {
            //cout << "found existing chunk" << endl;
            chunkXDifference++;
            chunkYDifference++;
            chunks[currentChunkIndex].neighborChunks[chunkYDifference * 3 + chunkXDifference] = i;
        }
    }
    //creating new chunks
    for (int i = 0; i < 9; i++)
    {
        if (chunks[currentChunkIndex].neighborChunks[i] == -1) 
        {
            int indexOfNewChunk = chunks.size();
            chunks[currentChunkIndex].neighborChunks[i] = indexOfNewChunk;
            chunks.push_back(Chunk(i % 3 - 1 + currentChunkX, i / 3 - 1 + currentChunkY, chunkSide, indexOfNewChunk, renderer)); 
        }
    }
    //for (int i = 0; i < 9; i++)
    //{
    //    cout << chunks[currentChunkIndex].neighborChunks[i] << "  ";
    //}
    //cout << currentChunkX << "  " << currentChunkY << endl;
    chunks[currentChunkIndex].isFilledNeighbors = true;
    return 0;
}