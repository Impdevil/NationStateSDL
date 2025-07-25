#ifndef MainGameLoop_H
#define MainGameLoop_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ChunkManager.h"

#include <unordered_map>
#include <memory>
#include "BaseObjects.h"

namespace MainGameLoop
{
    extern ChunkManager chunkManager;
    int initGame(SDL_Renderer *renderer);
    int MainGameLoop(float deltaTime);
    void RenderGameWorld(SDL_Renderer *renderer);
}

#endif // MainGameLoop_h