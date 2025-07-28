#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "CameraSystem.h"
#include <string>

class TextDisplay
{
private:
    TTF_Font *displayFont;
    SDL_FPoint origin;
    std::string label;
    SDL_Surface *surface;
    SDL_Texture *textTexture;
    SDL_FRect textRect;
    int fontsize;
    SDL_Color textColour;
    SDL_Renderer *renderer;
    bool worldSpaceText = false;
    CamStruct *camera;

public:
    TextDisplay() = default;
    TextDisplay(SDL_Renderer *ren, bool worldSpaceDisplay, float x, float y, TTF_Font *newFont, std::string lbl, SDL_Color colour);
    ~TextDisplay();
    void updateLabelText(std::string newText);
    void render(SDL_Renderer *renderer) const;
};

#endif