#ifndef StateManager_h
#define StateManager_h
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "CameraSystem.h"
#include "DebugTextDisplay.h"

enum GameState
{
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
};

class GameStateManager
{
public:
    bool debugMode = true;
    std::vector<DebugsDisplay*>  debugDisplays;
    
    GameStateManager();
    void setState(GameState newState);
    GameState getState() const;

    void moveToState(GameState newState, SDL_Renderer *renderer);
    void gameLoop(float deltaTime);
    void render(SDL_Renderer *renderer);
    TTF_Font *getDebugFont();
private:
    GameState currentState;
    TTF_Font *debugFont;
};

class StateManager
{
private:
    static StateManager *instance;
    GameStateManager gameStateManager;

    CamStruct camera; // Camera for rendering
    TTF_Font *debugFont;
    StateManager()
    {
        gameStateManager = GameStateManager();
        camera = CamStruct(0, 0, 800, 600, 1.0f); // Initialize camera with default values
        debugFont = TTF_OpenFont("assets/fonts/A25-SQUANOVA.ttf", 12);
    } // Initialize camera with default values

    void destroyInstance();

public:
    static StateManager *getInstance();
    GameStateManager *getGameStateManager();
    CamStruct *getCamera();
};

#endif // GameStateManager_h
