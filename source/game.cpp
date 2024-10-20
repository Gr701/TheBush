#include <stdio.h>
#include "game.h"   

Game::Game(SDL_Renderer* renderer, int FPS) : renderer(renderer) {
    frameDelay = 1000/FPS;

    int screenWidth, screenHeight; 
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    screenCenter = {screenWidth/2, screenHeight/2}; 

    player = Player(0, 0, scaleFactor, screenCenter, renderer);
    chunks.push_back(Chunk(0, 0, chunkSide, 0, renderer));
    currentChunkIndex = 0;
    fillNeighborChunks(&chunks[0]);
}

int Game::start() {
    bool quit = false;
    while (!quit) {   
        //MOVE 
        player.move();
        //LOGIC
        //if change chunk
        int chunkXDifference = player.coordinates.x / chunkSide - chunks[currentChunkIndex].chunkX;
        int chunkYDifference = player.coordinates.y / chunkSide - chunks[currentChunkIndex].chunkY;
        if (player.coordinates.x < 0) chunkXDifference--;
        if (player.coordinates.y < 0) chunkYDifference--;

        if (chunkXDifference != 0 || chunkYDifference != 0) {
            chunkXDifference++;
            chunkYDifference++;
            currentChunkIndex = chunks[currentChunkIndex].neighborChunks[3 * chunkYDifference + chunkXDifference];
            if (!chunks[currentChunkIndex].isFilledNeighbors) {
                fillNeighborChunks(&chunks[currentChunkIndex]);
            }
        }
        //INPUT HANDLING
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        //DRAW
        draw();

    }
    return 0;
}

int Game::draw() {
    SDL_SetRenderDrawColor(renderer, 64, 125, 91, 255); 
    SDL_RenderClear(renderer);
    //loaded chunks
    bool isPlayerDrawn = false;
    for (int chunkLine = 0; chunkLine < 3; chunkLine++) {
        Chunk& chunk0 = chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 0]]; //pointers to chunks from neigbor array of current chunk
        Chunk& chunk1 = chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 1]]; // these three chunks are in one horizontal line 
        Chunk& chunk2 = chunks[chunks[currentChunkIndex].neighborChunks[chunkLine * 3 + 2]];
        
        //Drawing chunk borders
        SDL_SetRenderDrawColor(renderer, (chunkLine + 1) * 80, 30, 190, 255);
        chunk0.draw(renderer, &player.coordinates, &screenCenter);
        chunk1.draw(renderer, &player.coordinates, &screenCenter);
        chunk2.draw(renderer, &player.coordinates, &screenCenter);

        int i = 0, j = 0, k = 0; //iterators for each of three chunks
        int object0Y = chunk0.objects[i].collistionRect.y + chunk0.objects[i].collistionRect.h;
        int object1Y = chunk1.objects[j].collistionRect.y + chunk1.objects[j].collistionRect.h;
        int object2Y = chunk2.objects[k].collistionRect.y + chunk2.objects[k].collistionRect.h;
        
        while (i < chunk0.objects.size() || j < chunk1.objects.size() || k < chunk2.objects.size()) {
            if (object0Y <= object1Y && object0Y <= object2Y) {
                drawOneObject(chunk0, i, object0Y, isPlayerDrawn);
            } else if (object1Y <= object0Y && object1Y <= object2Y) {
                drawOneObject(chunk1, j, object1Y, isPlayerDrawn);
            } else {
                drawOneObject(chunk2, k, object2Y, isPlayerDrawn);
            }
        }
    }
    //player.draw(renderer);
    SDL_RenderPresent(renderer); 
    return 0;
}

int Game::drawOneObject(Chunk& chunk, int& i, int& objectY, bool& isPLayerDrawn) {
    if (!isPLayerDrawn && objectY > player.collisionRect.y + player.collisionRect.h) {
        player.draw(renderer);
        isPLayerDrawn = true;
    }
    chunk.objects[i].draw(renderer, player.coordinates, screenCenter);
    i++;
    if (i < chunk.objects.size()) {
        objectY = chunk.objects[i].collistionRect.y + chunk.objects[i].collistionRect.h;
    } else {
        objectY += chunkSide << 2; //next obj Y or too big value when no more objects
    }
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