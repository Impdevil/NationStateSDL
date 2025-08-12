#ifndef TILEMAP_H
#define TILEMAP_H
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Tile.h"
#include <unordered_map>
#include <string>
#include "DebugTextDisplay.h"

struct ChunkCoord;
class ChunkManager;

class IsoTileMap : public renderableObject
{
public:
    IsoTileMap(SDL_Renderer *renderer, ChunkManager *manager, float x, float y, int width, int height, int tileWidth, int tileHeight);
    ~IsoTileMap();

    void render(SDL_Renderer *renderer) const override;
    void update(float deltaTime);
    void GenerateChunk(int width, int height, int tileWidth, int tileHeight);
    Tile CreateTile(int x, int y, int elevation, int tileWidth, int tileHeight, SDL_Color color, TILETYPE tType = TILETYPE::EMPTY);
    void SetTile(int x, int y, const Tile &tile);
    Tile* GetTile(int x, int y);
    void ClearTiles();

private:
    DebugTextMultiLine debugTileMapDisplay;
    SDL_FPoint origin;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    SDL_FRect tileMapRect; // Rect for the tile map
    SDL_Renderer *renderer;
    std::vector<std::vector<Tile>> tileMap; // 2D vector to hold the tile map
    ChunkManager *chunkManager;
    ChunkCoord *chunkCoord;
};
#endif // TileMap_h