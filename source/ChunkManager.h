#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "string"
#include "TileMap.h"

#include <unordered_map>

struct ChunkCoord
{
    int x, y;
    bool isSerialized;
    bool operator==(const ChunkCoord& coord) const {
        return x == coord.x && y == coord.y;
    }
};
namespace std {
    template <>
    struct hash <ChunkCoord>{
        std::size_t operator()(const ChunkCoord & coord) const {
            return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
        }
    };
}


class ChunkManager
{
private:
    int chunkCap = 10;
    std::unordered_map<ChunkCoord, IsoTileMap> chunks;           // Map to hold chunks by coordinates
    std::unordered_map<std::string, SDL_Texture*> textureCache; // Cache for textures to avoid reloading
    int chunkTileSizeX, chunkTileSizeY, chunkTileNumberX, chunkTileNumberY;
    std::vector<IsoTileMap*> chunkCacheRenders;
public:
    ChunkManager() = default;
    ~ChunkManager() = default;

    void init(SDL_Renderer *renderer);
    void update(float deltaTime); 
    std::vector<ChunkCoord> getActiveChunks(SDL_FPoint CameraLocation);
    std::vector<ChunkCoord> getChunksInView();
    IsoTileMap *getChunk(ChunkCoord coord);
    void GenerateChunk(ChunkCoord coord);
    void saveChunk(ChunkCoord coord);
    SDL_Texture* getTexture(std::string textureName);
    IsoTileMap* GetConnectedTileMap(SDL_FPoint origin, SDL_Point direction);
    std::vector<IsoTileMap*> getChunkCacheRenders(){
        if (chunkCacheRenders.empty())
        {
            return {};
        }
        return chunkCacheRenders;
    }

};

#endif