#include "TextDisplay.h"
#include "StateManager.h"
/// @brief used to create a line of text
/// @param ren
/// @param worldSpaceDisplay true uses world space, false stays attached to screen
/// @param x
/// @param y
/// @param newFont
/// @param lbl
/// @param colour
TextDisplay::TextDisplay(SDL_Renderer *ren, bool worldSpaceDisplay, float x, float y, TTF_Font *newFont, std::string lbl, SDL_Color colour)
    : renderer(ren), worldSpaceText(worldSpaceDisplay), origin{x, y}, label(lbl), textColour(colour)
{
    camera = StateManager::getInstance()->getCamera();
    displayFont = newFont;
    textRect = {x, y, 0, 0};
    surface = TTF_RenderText_Solid(displayFont, label.c_str(), 0, textColour);
    textTexture = SDL_CreateTextureFromSurface(ren, surface);

    textRect.w = surface->w;
    textRect.h = surface->h;
    SDL_DestroySurface(surface);
}
TextDisplay::~TextDisplay()
{
    SDL_DestroyTexture(textTexture);
    textTexture = nullptr;
}
void TextDisplay::updateLabelText(std::string newText)
{
    label = newText;
    SDL_DestroyTexture(textTexture);
    textTexture = nullptr;

    textRect = {origin.x, origin.y, 0, 0};
    surface = TTF_RenderText_Solid(displayFont, label.c_str(), 0, textColour);
    textTexture = SDL_CreateTextureFromSurface(renderer, surface);

    textRect.w = surface->w;
    textRect.h = surface->h;
    SDL_DestroySurface(surface);
}
void TextDisplay::render(SDL_Renderer *ren) const
{
    if (worldSpaceText)
    {
        if (!camera->inViewport(textRect))
            return;
        SDL_FRect screenSpaceRect = camera->getViewport();
        screenSpaceRect.x += textRect.x * camera->getZoom();
        screenSpaceRect.y += textRect.y * camera->getZoom();
        screenSpaceRect.w = textRect.w * camera->getZoom();
        screenSpaceRect.h = textRect.h * camera->getZoom();

        SDL_RenderTexture(ren, textTexture, nullptr, &screenSpaceRect);
    }
    else if (textTexture)
    {
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
    }
}