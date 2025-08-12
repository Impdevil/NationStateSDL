
#include "MainGameLoop.h"
//#include "ChunkManager.h"
#include "StateManager.h"
#include <cstdio>
#include "InputManager.h"
#include "DebugTextDisplay.h"


namespace MainGameLoop
{   
    DebugTextSingleLine debugCameraDisplay;
    ChunkManager chunkManager;
    std::vector<renderableObject*> renderableObjects = {};
    IsoTileMap *tileMap = nullptr;
    float tileRenderUpdateTimer;
    int initGame(SDL_Renderer *renderer)
    {
        debugCameraDisplay = DebugTextSingleLine(renderer,false,2,2,StateManager::getInstance()->getGameStateManager()->getDebugFont());
        debugCameraDisplay.addPropertyFRect("Camera pos", []() -> SDL_FRect {
            return StateManager::getInstance()->getCamera()->getViewport();
        });
        debugCameraDisplay.updateProperties();
        StateManager::getInstance()->getGameStateManager()->debugDisplays.emplace_back(std::make_unique<DebugTextSingleLine>(debugCameraDisplay));
        // create a tile object, i then what to create a tilemap, then i want to create a camera object for
        // moving around the tilemap and also not render the tiles that are not in the camera view
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

        // render textures here
        for (auto &objectToRender : renderableObjects)
        {
            objectToRender->render(renderer); // Render each renderable object
        }
        
    }
    void updateTileRenderCache(float deltaTime){
        tileRenderUpdateTimer += deltaTime;
        if (tileRenderUpdateTimer >= 30){
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
