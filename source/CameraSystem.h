#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H
#include <SDL3/SDL.h>
#include "InputManager.h"
#include <algorithm>

struct CamStruct
{
private:
    float x, y;          // Camera position
    float width, height; // Camera dimensions
    float widthResolution = 800; // Default width resolution
    float heightResolution = 600; // Default height resolution
    float margin = 10;
    float zoom;          // Zoom level
    float maxZoom = 2; // Maximum zoom level
    float minZoom = 0.2f; // Minimum zoom level
    SDL_FPoint lastmousePosition; // Mouse position relative to the camera
public:
    CamStruct(float x = 0, float y = 0, float width = 800, float height = 600, float zoom = 1.0f);

    void setPosition(float newX, float newY);

    void setZoom(float newZoom, SDL_FPoint focusPoint);
    float getZoom() const;
    SDL_FRect getViewport() const;

    bool inViewport(const SDL_FRect &rect) const;
    SDL_Point ScreenToWorld(const SDL_Point &screenPoint) const;
    SDL_Point WorldToScreen(const SDL_Point &worldPoint) const;

    void update(float deltaTime);
    
    
};

#endif // CAMERASYSTEM_H