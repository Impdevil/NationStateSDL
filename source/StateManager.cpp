
#include "StateManager.h"
#include "MainGameLoop.h"

#pragma region GameStateManager Implementation

GameStateManager::GameStateManager() : currentState(GAME_STATE_MENU)
{
    debugFont = TTF_OpenFont("assets/fonts/A25-SQUANOVA.ttf", 12);
    if (!debugFont)
    {
        SDL_Log("Failed to load debug font: %s", SDL_GetError());
    }
}
TTF_Font *GameStateManager::getDebugFont()
{
    return debugFont;
}

void GameStateManager::setState(GameState newState)
{
    currentState = newState;
}
GameState GameStateManager::getState() const
{
    return currentState;
}

void GameStateManager::moveToState(GameState newState, SDL_Renderer *renderer)
{
    // Handle any necessary transitions between states
    currentState = newState;
    switch (currentState)
    {
    case GameState::GAME_STATE_MENU:
        // Initialize menu state

        break;
    case GAME_STATE_PLAYING:
        // Initialize playing state
        MainGameLoop::initGame(renderer);
        break;
    case GAME_STATE_PAUSED:
        // Initialize paused state
        break;
    case GAME_STATE_GAME_OVER:
        // Initialize game over state
        break;
    }
}
void GameStateManager::gameLoop(float deltaTime)
{
    // update debug displays
    for (auto &debugDisplay : debugDisplays)
    {
        debugDisplay->updateProperties();
    }
    // Main game loop logic based on the current state
    switch (currentState)
    {
    case GAME_STATE_MENU:
        // Handle menu logic
        break;
    case GAME_STATE_PLAYING:
        MainGameLoop::MainGameLoop(deltaTime); // Call the main game loop logic
        break;
    case GAME_STATE_PAUSED:
        // Handle paused logic
        break;
    case GAME_STATE_GAME_OVER:
        // Handle game over logic
        break;
    }
}

void GameStateManager::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 89, 54, 54, 255);

    SDL_RenderClear(renderer);
    // Render based on the current state
    switch (currentState)
    {
    case GAME_STATE_MENU:
        // Render menu
        break;
    case GAME_STATE_PLAYING:
        MainGameLoop::RenderGameWorld(renderer);
        break;
    case GAME_STATE_PAUSED:
        // Render pause screen
        break;
    case GAME_STATE_GAME_OVER:
        // Render game over screen
        break;
    }
    for (auto &debugDisplay : debugDisplays)
    {
        debugDisplay->render(renderer);
    }
    SDL_RenderPresent(renderer);
}

#pragma endregion GameStateManager Implementation

#pragma region StateManager Implementation

StateManager *StateManager::instance = nullptr;

StateManager *StateManager::getInstance()
{
    if (!instance)
    {
        instance = new StateManager();
    }
    return instance;
}
void StateManager::destroyInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}
GameStateManager *StateManager::getGameStateManager()
{
    return &gameStateManager;
}
CamStruct *StateManager::getCamera()
{
    return &camera;
}

#pragma endregion StateManager Implementation
