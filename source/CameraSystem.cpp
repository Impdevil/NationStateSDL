
#include "CameraSystem.h"

CamStruct::CamStruct(float x, float y, float width, float height, float zoom)
    : x(x), y(y), width(width), height(height), zoom(zoom)
{
}

void CamStruct::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

void CamStruct::setZoom(float newZoom, SDL_FPoint focusPoint)
{
    zoom = std::clamp(newZoom, minZoom, maxZoom);
    float newWidth = widthResolution * zoom;
    float newHeight = heightResolution * zoom;

    x = focusPoint.x - (newWidth / 2);
    y = focusPoint.y - (newHeight / 2);
    width = newWidth;
    height = newHeight;
}

float CamStruct::getZoom() const
{
    return zoom;
}
bool CamStruct::inViewport(const SDL_FRect &rect) const
{
    SDL_FRect viewport = getViewport();
    return SDL_HasRectIntersectionFloat(&viewport, &rect);
}
SDL_FRect CamStruct::getViewport() const
{
    SDL_FRect viewport;
    viewport.x = x;
    viewport.y = y;
    viewport.w = width / zoom;
    viewport.h = height / zoom;
    return viewport;
}
SDL_Point CamStruct::ScreenToWorld(const SDL_Point &screenPoint) const
{
    // Convert screen coordinates to world coordinates based on camera position and zoom
    return {
        static_cast<int>((screenPoint.x - x) / zoom),
        static_cast<int>((screenPoint.y - y) / zoom)};
}
SDL_Point CamStruct::WorldToScreen(const SDL_Point &worldPoint) const
{
    // Convert world coordinates to screen coordinates based on camera position and zoom
    return {
        static_cast<int>(worldPoint.x * zoom + x),
        static_cast<int>(worldPoint.y * zoom + y)};
}

void CamStruct::update(float deltaTime)
{
    // move camera with mouse left button pressed

    if (InputManager::GetInstance()->GetMouseInfo().mouseButtonStates[SDL_BUTTON_LEFT] == BUTTONSTATE::JUSTPRESSED)
    {
        lastmousePosition.x = InputManager::GetInstance()->GetMouseInfo().x;
        lastmousePosition.y = InputManager::GetInstance()->GetMouseInfo().y;
    }
    else if (InputManager::GetInstance()->GetMouseInfo().mouseButtonStates[SDL_BUTTON_LEFT] == BUTTONSTATE::HELD)
    {
        float deltaX = (lastmousePosition.x - InputManager::GetInstance()->GetMouseInfo().x);
        float deltaY = (lastmousePosition.y - InputManager::GetInstance()->GetMouseInfo().y);
        // log the delta values
        // SDL_Log("Camera deltaX: %f, deltaY: %f", deltaX, deltaY);
        // SDL_Log("Camera position before update: x: %f, y: %f", x, y);
        // Update camera position based on mouse movement
        //deltaX /= zoom; // Adjust for zoom level
        //deltaY /= zoom; // Adjust for zoom level

        setPosition(x + deltaX * -.01f, y + deltaY * -.01f);
        // SDL_Log("Camera deltaX: %f, deltaY: %f after ", deltaX * -.01f, deltaY * -.01f);
        // SDL_Log("Camera position after update: x: %f, y: %f", x, y);
    }
    // middle mouse scroll zoom
    if (InputManager::GetInstance()->GetMouseInfo().scrollDirY != SCROLL_NONE)
    {
        lastmousePosition.x = InputManager::GetInstance()->GetMouseInfo().x;
        lastmousePosition.y = InputManager::GetInstance()->GetMouseInfo().y;
        float scrollAmount = 0.01f; // Adjust zoom sensitivity

        if (InputManager::GetInstance()->GetMouseInfo().scrollDirY == SCROLL_UP)
        {
            scrollAmount = 0.05;
        }
        else if (InputManager::GetInstance()->GetMouseInfo().scrollDirY == SCROLL_DOWN)
        {
            scrollAmount = -0.05f;
        }
        setZoom(zoom + scrollAmount, lastmousePosition);
        SDL_Log("Camera zoom changed to: %f", zoom);
    }
}
