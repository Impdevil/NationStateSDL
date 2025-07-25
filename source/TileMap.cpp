#include "TileMap.h"
#include "StateManager.h"
#include "ChunkManager.h"

IsoTileMap::IsoTileMap(SDL_Renderer *renderer, ChunkManager *manager, float tilemapX, float tilemapY, int width, int height, int tileWidth, int tileHeight)
    : renderer(renderer), chunkManager(manager), origin{tilemapX, tilemapY}, width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight)
{
    tileMapRect = {tilemapX, tilemapY, static_cast<float>(width * tileWidth), static_cast<float>(height * tileHeight)};
    if (!chunkManager->getTexture("default"))
    {
        SDL_Log("Failed to load default texture: %s", SDL_GetError());
    }
    GenerateChunk(width, height, tileWidth, tileHeight);
}

IsoTileMap::~IsoTileMap()
{
    ClearTiles();
}
void IsoTileMap::render(SDL_Renderer *renderer) const
{
    if (StateManager::getInstance()->getCamera()->inViewport(tileMapRect))
    {
        for (const auto &row : tileMap)
        {
            for (const auto &tile : row)
            {
                tile.render(renderer);
            }
        }
    }
}
void IsoTileMap::update(float deltaTime)
{
    // Update logic for the tile map can be added here
}

void IsoTileMap::GenerateChunk(int width, int height, int tileWidth, int tileHeight)
{
    tileMap.resize(height);
    for (int y = 0; y < height; ++y)
    {
        tileMap[y].reserve(width);
        for (int x = 0; x < width; ++x)
        {
            SDL_Color color = {255, 255, 255, 255}; // Default color for tiles
            SDL_Log("Creating tile at iso position (%f, %f) with size (%d, %d)", (x + origin.x * width), (y + origin.y * height), tileWidth, tileHeight);

            tileMap[y].emplace_back(CreateTile(x, y, 0, tileWidth, tileHeight, color, TILETYPE::GRASS));
        }
    }
}

// TODO:::::
// change the get tile to get the actual tile location not the shifted pixel position
Tile IsoTileMap::CreateTile(int x, int y, int elevation, int tileWidth, int tileHeight, SDL_Color color, TILETYPE tType)
{
    float shiftedX = x + origin.x * width;
    float shiftedY = y + origin.y * height;
    float isoX = (shiftedX - shiftedY) * (tileWidth / 2);
    float isoY = (shiftedX + shiftedY) * (tileHeight / 2);
    Tile newTile = Tile(renderer, x,y,(float)isoX, (float)isoY, elevation, (float)tileWidth, (float)tileHeight, chunkManager->getTexture("default"), tType);
    newTile.setTileConnnections(this, GetTile(x, y - 1), GetTile(x + 1, y), GetTile(x, y + 1), GetTile(x - 1, y));

    SDL_Log("creating tile 2");
    return newTile;
}
void IsoTileMap::SetTile(int x, int y, const Tile &tile)
{
    if (x >= 0 && x < tileMap[0].size() && y >= 0 && y < tileMap.size())
    {
        tileMap[y][x] = tile;
    }
}
Tile *IsoTileMap::GetTile(int x, int y)
{
    if (y >= 0 && y < tileMap.size() && x >= 0 && x < tileMap[y].size())
    {
        SDL_Log("Tile found");
        return &tileMap[y][x];
    }
    SDL_Point direction;
    if (x < 0)
    {
        direction.x = -1;
    }
    else if (x > tileMap.size())
    {
        direction.x = 1;
    }
    if (y < 0)
    {
        direction.y = -1;
    }
    else if (y > tileMap.size())
    {
        direction.y = 1;
    }
    if (chunkManager)
    {
        // chunkManager->GetConnectedTileMap(origin, direction)->GetTile(width-x,height-y);
        SDL_Log("trying to connect to separate chunk (%f , %f) to  (%f, %f)", origin.x, origin.y, direction.x, direction.y);
        IsoTileMap *ConnectedChunk = chunkManager->GetConnectedTileMap(origin, direction);
        if (ConnectedChunk)
        {
            Tile *connectedTile = ConnectedChunk->GetTile(width - x, height - y);
            if (connectedTile)
                return connectedTile;
        }
    }
    SDL_Log("Connected Tile does not exist");
    return nullptr;
}
void IsoTileMap::ClearTiles()
{
    tileMap.clear();
}
