#ifndef TEXTURE2D_H
#define TEXTURE2D_H
// #pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "CameraSystem.h"

struct cameraViewStruct{
    float x,y,w,h;

    
};

struct Texture2D
{
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_FRect destRect_worldSpace;
    SDL_FPoint center = {0, 0};             // Center for rotation
    SDL_Color color = {255, 255, 255, 255}; // Default color
    float angle = 0.0f;
    bool ownTexture = true; // Flag to indicate if the texture should be destroyed in the destructor
    CamStruct *camera;
    
    void init();
    Texture2D();

    Texture2D(SDL_Renderer *ren, const char *filePath, SDL_FRect *rect) ;

    Texture2D(SDL_Renderer *ren, const char *filePath, float x, float y, float width, float hight) ;

    Texture2D(SDL_Renderer *ren, SDL_Texture *txtr, float x, float y, float width, float hight);

    ~Texture2D();

    void render() const;
    void render(SDL_Renderer *ren) const;

    void resize(int width, int height);
    void Rotate(float angle);
    void Move(float x, float y);
    SDL_Point getWorldPosition() const;
};

#endif // TEXTURE2D_H