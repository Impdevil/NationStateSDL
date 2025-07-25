#ifndef TILE_H
#define TILE_H

#include <SDL3/SDL.h>
#include <vector>
#include "Texture2D.h"
#include "BaseObjects.h"

enum TILEDIRECTION
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

// Enum for tile types
enum class TILETYPE
{
    GRASS,
    WATER,
    SAND,
    ROCK,
    FOREST,
    ROAD,
    BUILDING,
    EMPTY // Default type for empty tiles
};
class IsoTileMap;

class Tile : public renderableObject
{
private:
    float isoX, isoY, width, height;
    SDL_FPoint origin;
    int elevation = 0;
    SDL_Color colour;
    Texture2D tileTexture;
    Texture2D edgeTexture;
    TILETYPE tType = TILETYPE::EMPTY;    // Default tile type
    Tile *connectedTiles[4] = {nullptr};  // Array to hold connected tiles (N, E, S, W)
    IsoTileMap *parentTileMap = nullptr; // Pointer to the parent tile map for context

public:
    Tile(SDL_Renderer *ren);
    Tile(SDL_Renderer *ren,float x,float y, float isoX, float isoY, float width, float height, SDL_Color color);
    Tile(SDL_Renderer *ren,float x,float y, float isoX, float isoY, int elev, float width, float height, SDL_Texture *txtr, TILETYPE tType);
    ~Tile();
    void setTileType(SDL_Renderer *ren, const char *filePath);
    void setTileConnnections(IsoTileMap *parent, Tile *north, Tile *east, Tile *south, Tile *west);

    void setPosition(float x, float isoY);
    int getElevation() const;

    void render(SDL_Renderer *renderer) const override;
};

#endif // tile_h