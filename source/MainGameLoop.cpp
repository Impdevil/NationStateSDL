
#include "MainGameLoop.h"
//#include "ChunkManager.h"
#include "StateManager.h"
#include <cstdio>
#include "InputManager.h"


namespace MainGameLoop
{
    ChunkManager chunkManager;
    std::unordered_map<int, std::unique_ptr<renderableObject>> renderableObjects;
    IsoTileMap *tileMap = nullptr;
    int initGame(SDL_Renderer *renderer)
    {
        // create a tile object, i then what to create a tilemap, then i want to create a camera object for
        //  moving around the tilemap and also not render the tiles that are not in the camera view
        printf("Initializing main game...\n");
        SDL_Color tileColor = {255, 255, 255, 255};
        chunkManager = ChunkManager();
        chunkManager.init(renderer);
        // renderableObjects[3] = std::make_unique<IsoTileMap>(renderer,chunkManager, 0, 0,10,10, 64, 32);
        // renderableObjects[2] = std::make_unique<IsoTileMap>(renderer,chunkManager, 0, 1,10,10, 64, 32);
        // renderableObjects[1] = std::make_unique<IsoTileMap>(renderer,chunkManager, 1, 0,10,10, 64, 32);
        // renderableObjects[0] = std::make_unique<IsoTileMap>(renderer,chunkManager, 1, 1,10,10, 64, 32);
        return 0;
    }
    int MainGameLoop(float deltaTime)
    {

        StateManager::getInstance()->getCamera()->update(deltaTime);

        return 0;
    }
    void RenderGameWorld(SDL_Renderer *renderer)
    {
        // Set the draw color for the renderer
        SDL_SetRenderDrawColor(renderer, 89, 54, 54, 255);

        SDL_RenderClear(renderer);
        // render textures here

        for (const auto &pair : renderableObjects)
        {
            pair.second->render(renderer); // Render each renderable object
        }

        SDL_RenderPresent(renderer);
    }
}
