#include "Texture2D.h"
#include "StateManager.h"


void Texture2D::init()
{
    camera = StateManager::getInstance()->getCamera();
}

Texture2D::Texture2D()
{
    // Default constructor
    texture = nullptr;
    renderer = nullptr;
    destRect_worldSpace = {0, 0, 0, 0};
    center = {0, 0};
    angle = 0.0f;
    //SDL_Log("Render texture constructor 0");
    init();
}

Texture2D::Texture2D(SDL_Renderer *ren, const char *filePath, SDL_FRect *rect) : renderer(ren), texture(nullptr), destRect_worldSpace(*rect)
{
    init();
    texture = IMG_LoadTexture(renderer, filePath);
    center.x = destRect_worldSpace.w / 2;
    center.y = destRect_worldSpace.h / 2;
    if (!texture)
    {
        SDL_Log("Could not load texture: %s\n", SDL_GetError());
    }
    SDL_Log("Render texture1");
}
Texture2D::Texture2D(SDL_Renderer *ren, const char *filePath, float x, float y, float width, float hight) : renderer(ren), texture(nullptr), destRect_worldSpace({x, y, width, hight})
{
    init();
    texture = IMG_LoadTexture(renderer, filePath);
    center.x = destRect_worldSpace.w / 2;
    center.y = destRect_worldSpace.h / 2;
    if (!texture)
    {
        SDL_Log("Could not load texture: %s\n", SDL_GetError());
    }
    //SDL_Log("Render texture2");
}
Texture2D::Texture2D(SDL_Renderer *ren, SDL_Texture *txtr, float x, float y, float width, float hight) : renderer(ren), ownTexture(false)
{
    init();
    texture = txtr;
    destRect_worldSpace = SDL_FRect{x, y, width, hight};

    if (!texture)
    {
        SDL_Log("Could not load texture: %s\n", SDL_GetError());
    }
    //SDL_Log("Render texture3 with texture from pointer %p", texture);

    center.x = destRect_worldSpace.w / 2;
    center.y = destRect_worldSpace.h / 2;
}

Texture2D::~Texture2D()
{
    if (texture && ownTexture)
    {
        SDL_DestroyTexture(texture);
    }
}

void Texture2D::render() const
{
    if (texture || renderer)
    {
        if (angle != 0.0f)
        {
            SDL_RenderTextureRotated(renderer, texture, nullptr, &destRect_worldSpace, angle, &center, SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderTexture(renderer, texture, NULL, &destRect_worldSpace);
        }
        // SDL_Log("Rendering texture at position (%f, %f) with size (%f, %f)", destRect.x, destRect.y, destRect.w, destRect.h);
        // if (SDL_GetError() != "")
        // {
        //     SDL_Log("Texture not loaded (unreq Render): %s\n", SDL_GetError());
        // }
        return;
    }
    SDL_Log("Texture not loaded(unreq Ren): %s\n", SDL_GetError());
}
void Texture2D::render(SDL_Renderer *ren) const
{
    if (!camera->inViewport(destRect_worldSpace))
    {
        if (!ren)
        {
            SDL_Log("Renderer is null, cannot render texture.");
            return;
        }
        return;
    }
    SDL_FRect screenSpaceRect = camera->getViewport();
    screenSpaceRect.x *= -1;
    screenSpaceRect.y *= -1;
    screenSpaceRect.x += destRect_worldSpace.x * camera->getZoom() ;
    screenSpaceRect.y += destRect_worldSpace.y * camera->getZoom();
    screenSpaceRect.w = destRect_worldSpace.w * camera->getZoom();
    screenSpaceRect.h = destRect_worldSpace.h * camera->getZoom();
    //SDL_Log("screnen rect: x %f, y %f, w %f, h%f", screenSpaceRect.x,screenSpaceRect.y,screenSpaceRect.w,screenSpaceRect.h);
    if (texture)
    {
        if (angle != 0.0f)
        {
            SDL_RenderTextureRotated(ren, texture, nullptr, &screenSpaceRect, angle, &center, SDL_FLIP_NONE);
        }
        else
        {
            SDL_RenderTexture(ren, texture, NULL, &screenSpaceRect);
        }
        // SDL_Log("Rendering texture at position (%f, %f) with size (%f, %f)", destRect.x, destRect.y, destRect.w, destRect.h);
        // if (SDL_GetError() != "")
        // {
        //     SDL_Log("Texture not loaded (req Render): %s\n", SDL_GetError());
        // }
        // SDL_Log("Rendering texture at position (%f, %f) with size (%f, %f)", destRect.x, destRect.y, destRect.w, destRect.h);

        return;
    }
    //SDL_Log("Texture not loaded (req Render): %s\n", SDL_GetError());
}

void Texture2D::resize(int width, int height)
{
    if (texture)
    {
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
        destRect_worldSpace.w = width;
        destRect_worldSpace.h = height;
    }
}
void Texture2D::Rotate(float angle)
{
    if (texture)
    {
        center = {destRect_worldSpace.w / 2, destRect_worldSpace.h / 2};
        this->angle = angle;
    }
}
void Texture2D::Move(float x, float y)
{
    if (texture)
    {
        destRect_worldSpace.x = x;
        destRect_worldSpace.y = y;
        center = {destRect_worldSpace.w / 2, destRect_worldSpace.h / 2};
    }
}
SDL_Point Texture2D::getWorldPosition() const
{
    return {static_cast<int>(destRect_worldSpace.x + center.x), static_cast<int>(destRect_worldSpace.y + center.y)};
}
