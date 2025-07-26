#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <cstdio>
#include <unistd.h>
#include "StateManager.h"
#include "InputManager.h"

int main(int argc, char *argv[])
{
    printf("Starting SDL3 application...\n");
    if (SDL_Init(SDL_INIT_VIDEO) != true)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL3 Window", 800, 600, 0);
    if (!window)
    {
        printf("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        printf("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    StateManager &stateManager = *StateManager::getInstance();
    InputManager *inputManager = InputManager::GetInstance();
    inputManager->initButton();
    stateManager.getGameStateManager()->setState(GAME_STATE_MENU);
    stateManager.getGameStateManager()->moveToState(GAME_STATE_PLAYING, renderer);
    static Uint64 lastCounter = SDL_GetTicks();
    Uint64 currentCounter = SDL_GetTicks();


    bool running = true;
    int i = 0;
    while (inputManager->Running)
    {
        currentCounter = SDL_GetTicks();
        float deltaTime = (currentCounter - lastCounter); // Milliseconds
        lastCounter = currentCounter;
        //SDL_Log("Delta Time: %f seconds", deltaTime);
        inputManager->ProcessInput();

        stateManager.getGameStateManager()->gameLoop(deltaTime); // Assuming a fixed delta time for simplicity
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        stateManager.getGameStateManager()->render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}