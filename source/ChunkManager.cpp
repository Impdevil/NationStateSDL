#include "ChunkManager.h"
#include "StateManager.h"
#include <cmath>

void ChunkManager::init(SDL_Renderer *renderer)
{
    // Initialize the chunk manager, if needed
    SDL_Log("ChunkManager initialized.");
    chunkTileSizeX = 64;
    chunkTileNumberX = 10;
    chunkTileSizeY = 32;
    chunkTileNumberY = 10;

    textureCache.reserve(100); // Reserve space for texture cache to avoid frequent reallocations
    textureCache["default"] = IMG_LoadTexture(renderer, "assets/images/worldTextures/grasstexture1.png");
    for (int x = 0-chunkCap/2; x <= 0+chunkCap/2; x++){
        for (int y = 0-chunkCap/2; y < 0+chunkCap/2; y++)
        {
            ChunkCoord newchunk = {x, y, false};
            
            chunks.insert({newchunk, IsoTileMap (renderer, this, x, y, chunkTileNumberX, chunkTileNumberY, chunkTileSizeX, chunkTileSizeY)});
        }
    }
    getChunksInView();
}
void ChunkManager::update(float deltaTime)
{
    // Update logic for chunks, if needed
    SDL_Log("ChunkManager updated with deltaTime: %f", deltaTime);
}
std::vector<ChunkCoord> ChunkManager::getActiveChunks(SDL_FPoint CameraLocation){
    return {};
}
std::vector<ChunkCoord> ChunkManager::getChunksInView(){
    std::vector<ChunkCoord> results{};
    chunkCacheRenders = {};
    CamStruct* cam = StateManager::getInstance()->getCamera();
    SDL_FRect viewport = cam->getViewport();
    int minChunkX = floor(viewport.x/(chunkTileSizeX*chunkTileNumberX));
    int maxChunkx = ceil((viewport.x + viewport.w)/( chunkTileSizeX*chunkTileNumberX));

    int minChunkY = floor (viewport.y/(chunkTileSizeY*chunkTileNumberY));
    int maxChunkY = ceil ((viewport.y+viewport.h)/(chunkTileSizeY*chunkTileNumberY));

    for (int cy = minChunkY; cy <= maxChunkY; ++cy)
        for (int cx =minChunkX; cx <= maxChunkx; ++cx){
            if (chunks.find({cx,cy}) != chunks.end())
            {
                results.push_back({cx,cy});
                //if (std::find(chunkCacheRenders.begin(),chunkCacheRenders.end(),&chunks.at({cx,cy})) == chunkCacheRenders.end())
                chunkCacheRenders.push_back(&chunks.at({cx,cy}));
            }
        }
    return results;
}


IsoTileMap *ChunkManager::getChunk(ChunkCoord coord)
{
    // Logic to retrieve a chunk at the specified coordinates
    SDL_Log("Retrieving chunk at coordinates (%d, %d)", coord.x, coord.y);
    IsoTileMap *map = &chunks.at(coord);
    return map;
}
void ChunkManager::GenerateChunk(ChunkCoord coord)
{
    // Logic to generate a chunk at the specified coordinates
    SDL_Log("Generating chunk at coordinates (%d, %d)", coord.x, coord.y);
}
void ChunkManager::saveChunk(ChunkCoord coord)
{
    // Logic to save the chunk to a file or database
    SDL_Log("Saving chunk at coordinates (%d, %d)", coord.x, coord.y);
    coord.isSerialized = true; // Mark the chunk as serialized
}
SDL_Texture *ChunkManager::getTexture(std::string textureName)
{
    return textureCache.at(textureName);
}
IsoTileMap *ChunkManager::GetConnectedTileMap(SDL_FPoint origin, SDL_Point direction)
{
    return nullptr;
}

