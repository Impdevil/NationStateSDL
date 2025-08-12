#include "TileMap.h"
#include "StateManager.h"
#include "ChunkManager.h"
#include <memory>

IsoTileMap::IsoTileMap(SDL_Renderer *renderer, ChunkManager *manager, float tilemapX, float tilemapY, int width, int height, int tileWidth, int tileHeight)
    : renderer(renderer), chunkManager(manager), origin{tilemapX, tilemapY}, width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight)
{ //-5*10*64
    float chunkwidth = width * tileWidth;
    float chunkHeight = height * tileHeight;
    float isoX = (tilemapX - tilemapY) * (chunkwidth / 2);
    float isoY = (tilemapX + tilemapY) * (chunkHeight / 2);

    float shiftedX = isoX + origin.x;
    float shiftedY = isoY + origin.y;

    origin = {shiftedX, shiftedY};
    tileMapRect = {origin.x, origin.y, chunkwidth, chunkHeight};
    if (!chunkManager->getTexture("default"))
    {
        SDL_Log("Failed to load default texture: %s", SDL_GetError());
    }
    auto capturedOrigin = origin;

    debugTileMapDisplay = DebugTextMultiLine(renderer, true, origin.x /*+(chunkwidth/2), origin.y + (chunkHeight/2)*/, origin.y, StateManager::getInstance()->getGameStateManager()->getDebugFont());
    debugTileMapDisplay.addPropertyOrigin("TileMap Origin", [capturedOrigin]() -> SDL_FPoint
                                          { return capturedOrigin; });
    SDL_FPoint CapturetileMapPosition = {tilemapX, tilemapY};
    debugTileMapDisplay.addPropertyOrigin("TileMap Position", [CapturetileMapPosition]() -> SDL_FPoint
                                          { return CapturetileMapPosition; });

    StateManager::getInstance()->getGameStateManager()->debugDisplays.push_back(std::make_unique<DebugTextMultiLine>(debugTileMapDisplay));
    GenerateChunk(width, height, tileWidth, tileHeight);
}

IsoTileMap::~IsoTileMap()
{
    ClearTiles();
}
void IsoTileMap::render(SDL_Renderer *renderer) const
{

    for (const auto &row : tileMap)
    {
        for (const auto &tile : row)
        {
            tile.render(renderer);
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
    for (int offsetTileY = 0; offsetTileY < height; ++offsetTileY)
    {
        tileMap[offsetTileY].reserve(width);
        for (int offsetTileX = 0; offsetTileX < width; ++offsetTileX)
        {
            SDL_Color color = {255, 255, 255, 255}; // Default color for tiles
            // SDL_Log("Creating tile at iso position (%f, %f) with size (%d, %d)", (x + origin.x * width), (y + origin.y * height), tileWidth, tileHeight);

            tileMap[offsetTileY].emplace_back(CreateTile(offsetTileX, offsetTileY, 0, tileWidth, tileHeight, color, TILETYPE::GRASS));
        }
    }
}

Tile IsoTileMap::CreateTile(int offsetTileX, int offsetTileY, int elevation, int tileWidth, int tileHeight, SDL_Color color, TILETYPE tType)
{

    float isoX = (offsetTileX - offsetTileY) * (tileWidth / 2);
    float isoY = (offsetTileX + offsetTileY) * (tileHeight / 2);

    float shiftedX = isoX + origin.x;
    float shiftedY = isoY + origin.y;
    Tile newTile = Tile(renderer, offsetTileX, offsetTileY, (float)shiftedX, (float)shiftedY, elevation, (float)tileWidth, (float)tileHeight, chunkManager->getTexture("default"), tType);
    newTile.setTileConnnections(this, GetTile(offsetTileX, offsetTileY - 1), GetTile(offsetTileX + 1, offsetTileY), GetTile(offsetTileX, offsetTileY + 1), GetTile(offsetTileX - 1, offsetTileY));

    // SDL_Log("creating tile 2");
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
        // SDL_Log("Tile found");
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
        // SDL_Log("trying to connect to separate chunk (%f , %f) to  (%f, %f)", origin.x, origin.y, direction.x, direction.y);
        IsoTileMap *ConnectedChunk = chunkManager->GetConnectedTileMap(origin, direction);
        if (ConnectedChunk)
        {
            Tile *connectedTile = ConnectedChunk->GetTile(width - x, height - y);
            if (connectedTile)
                return connectedTile;
        }
    }
    // SDL_Log("Connected Tile does not exist");
    return nullptr;
}
void IsoTileMap::ClearTiles()
{
    tileMap.clear();
}
