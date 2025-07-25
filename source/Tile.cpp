#include "Tile.h"

Tile::Tile(SDL_Renderer *ren) : origin{0,0}, isoX(0), isoY(0), width(0), height(0), colour({0, 0, 0, 0}), tType(TILETYPE::EMPTY)
{
    //SDL_Log("Creating (0)Tile at position (%f, %f) with size (%f, %f)", isoX, isoY , width, height);
}

Tile::Tile(SDL_Renderer *ren,float x,float y, float isox ,float isoy, float width, float height, SDL_Color color)
    : origin{x,y} ,isoX(isox), isoY(isoy), width(width), height(height), colour(color), tileTexture(ren, "assets/images/worldTextures/grasstexture1.png", x, y, width, height)
{
    tileTexture = Texture2D(ren, "assets/images/worldTextures/grasstexture1.png", new SDL_FRect{x, y, width, height});
    //SDL_Log("Creating (1)Tile at position (%f, %f) with size (%f, %f)", x, y, width, height);
}
Tile::Tile(SDL_Renderer *ren,float x,float y, float isox, float isoy, int elev, float width, float height, SDL_Texture *txtr, TILETYPE tType)
    : origin{x,y}, isoX(isox), isoY(isoy), elevation(elev), width(0), height(0), tileTexture(ren, txtr, x, y, width, height), tType(tType)
{
    //SDL_Log("Creating (2)Tile at position (%f, %f) with size (%f, %f)", x, y, width, height);
}
Tile::~Tile()
{
    tileTexture.~Texture2D();
}
void Tile::setTileType(SDL_Renderer *ren, const char *filePath)
{
    tileTexture = Texture2D(ren, filePath, new SDL_FRect{isoX, isoY, width, height});
}
void Tile::setTileConnnections(IsoTileMap *parent, Tile *north, Tile *east, Tile *south, Tile *west)
{
    if (parent)
        parentTileMap = parent;
    if (!connectedTiles[NORTH] && north)
    {
        connectedTiles[NORTH] = north;
        north->setTileConnnections(nullptr, nullptr, nullptr, this, nullptr);
    }
    if (!connectedTiles[EAST] && east)
        connectedTiles[EAST] = east;
    if (!connectedTiles[SOUTH] && south)
        connectedTiles[SOUTH] = south;
    if (!connectedTiles[WEST] && west)
    {
        connectedTiles[WEST] = west;
        west->setTileConnnections(nullptr, nullptr, this, nullptr, nullptr);
    }
}

void Tile::setPosition(float x, float y)
{
    this->isoX = x;
    this->isoY = y;
    tileTexture.destRect_worldSpace.x = x;
    tileTexture.destRect_worldSpace.y = y - elevation;
}
int Tile::getElevation() const
{
    return elevation;
}

void Tile::render(SDL_Renderer *renderer) const
{
    if (elevation != 0)
    {
    }

    tileTexture.render(renderer);
}
