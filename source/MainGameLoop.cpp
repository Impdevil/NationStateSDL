
#include "MainGameLoop.h"
//#include "ChunkManager.h"
#include "StateManager.h"
#include <cstdio>
#include "InputManager.h"


namespace MainGameLoop
{
    ChunkManager chunkManager;
    std::vector<renderableObject*> renderableObjects = {};
    IsoTileMap *tileMap = nullptr;
    float tileRenderUpdateTimer;
    int initGame(SDL_Renderer *renderer)
    {
        // create a tile object, i then what to create a tilemap, then i want to create a camera object for
        //  moving around the tilemap and also not render the tiles that are not in the camera view
        printf("Initializing main game...\n");
        SDL_Color tileColor = {255, 255, 255, 255};
        chunkManager = ChunkManager();
        chunkManager.init(renderer);
        updateTileRenderCache(16);
        SDL_Log("renderableObjects size %d", renderableObjects.size()  );
        return 0;
    }
    int MainGameLoop(float deltaTime)
    {

        StateManager::getInstance()->getCamera()->update(deltaTime);
        
        
        
        updateTileRenderCache(deltaTime);

        return 0;
    }
    void RenderGameWorld(SDL_Renderer *renderer)
    {  

        // Set the draw color for the renderer
        SDL_SetRenderDrawColor(renderer, 89, 54, 54, 255);

        SDL_RenderClear(renderer);
        // render textures here
        int i= 0;
        for (auto &objectToRender : renderableObjects)
        {
            ++i;
            objectToRender->render(renderer); // Render each renderable object
            //SDL_Log("loops = %d ", i);
        }
        
        SDL_RenderPresent(renderer);
    }
    void updateTileRenderCache(float deltaTime){
        tileRenderUpdateTimer += deltaTime;
        if (tileRenderUpdateTimer >= 16){
            clearRenderStream();
            tileRenderUpdateTimer = 0;
            chunkManager.getChunksInView();
            auto  chunkCache = chunkManager.getChunkCacheRenders();
            renderableObjects.insert(renderableObjects.end(),chunkCache.begin(),chunkCache.end());
        }
    }
    void clearRenderStream(){
        renderableObjects.clear();
    }
}
