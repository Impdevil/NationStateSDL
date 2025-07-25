
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

void CamStruct::setZoom(float newZoom)
{
    zoom = std::clamp(newZoom, minZoom, maxZoom);
}

float CamStruct::getZoom() const
{
    return zoom;
}
bool CamStruct::inViewport(const SDL_FRect &rect) const
{
    // Check if the rectangle is within the camera viewport
    return (rect.x + rect.w > x && rect.x < x + width * zoom + margin||
            rect.y + rect.h > y && rect.y < y + height * zoom + margin);
}
SDL_FRect CamStruct::getViewport() const{
    return {x,y,width*zoom+ margin, height*zoom+ margin};
}
SDL_Point CamStruct::ScreenToWorld(const SDL_Point &screenPoint) const
{
    // Convert screen coordinates to world coordinates based on camera position and zoom
    return {
        static_cast<int>((screenPoint.x - x) / zoom),
        static_cast<int>((screenPoint.y - y) / zoom)
    };
}
SDL_Point CamStruct::WorldToScreen(const SDL_Point &worldPoint) const
{
    // Convert world coordinates to screen coordinates based on camera position and zoom
    return {
        static_cast<int>(worldPoint.x * zoom + x),
        static_cast<int>(worldPoint.y * zoom + y)
    };
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
        deltaX *= zoom; // Adjust for zoom level
        deltaY *= zoom; // Adjust for zoom level

        setPosition(x + deltaX * -.01f, y + deltaY * -.01f);
        // SDL_Log("Camera deltaX: %f, deltaY: %f after ", deltaX * -.01f, deltaY * -.01f);
        // SDL_Log("Camera position after update: x: %f, y: %f", x, y);
    }
    // middle mouse scroll zoom
    if (InputManager::GetInstance()->GetMouseInfo().wheelY > 0 || InputManager::GetInstance()->GetMouseInfo().wheelY < 0)
    {
        float scrollAmount = InputManager::GetInstance()->GetMouseInfo().wheelY * 0.001f; // Adjust zoom sensitivity
        setZoom(zoom + scrollAmount);
        SDL_Log("Camera zoom changed to: %f", zoom);
    }
}
