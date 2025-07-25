#include "InputManager.h"

InputManager *InputManager::instance = nullptr;

InputManager *InputManager::GetInstance()
{
    if (!instance)
    {
        instance = new InputManager();
    }
    return instance;
}
InputManager::InputManager()
{
}
void InputManager::initButton()
{
    keyboardStates[SDLK_LEFT] = BUTTONSTATE::RELEASED;
    keyboardStates[SDLK_RIGHT] = BUTTONSTATE::RELEASED;
    keyboardStates[SDLK_DOWN] = BUTTONSTATE::RELEASED;
    keyboardStates[SDLK_UP] = BUTTONSTATE::RELEASED;

    mouseButtonStates[SDL_BUTTON_LEFT] = BUTTONSTATE::RELEASED;
    mouseButtonStates[SDL_BUTTON_RIGHT] = BUTTONSTATE::RELEASED;
    mouseButtonStates[SDL_BUTTON_MIDDLE] = BUTTONSTATE::RELEASED;

    activeFingers.clear(); // Clear any active fingers at initialization
}
void InputManager::ProcessInput()
{
    for (auto &[key, state] : keyboardStates)
    {
        if (state == JUSTPRESSED)
            state = HELD;
        else if (state == JUSTRELEASED)
            state = RELEASED;
    }
    for (auto &[key, state] : mouseButtonStates)
    {
        if (state == JUSTPRESSED)
            state = HELD;
        else if (state == JUSTRELEASED)
            state = RELEASED;
    }
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            // Handle quit event
            instance->GetInstance()->~InputManager();
            Running = false; // Set a flag to stop the main loop
            break;
        case SDL_EVENT_KEY_DOWN:
            updateKeyButtonState(event.key.key, true);
            break;
        case SDL_EVENT_KEY_UP:
            updateKeyButtonState(event.key.key, false);
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            updateMouseButtonState(event.button.button, true);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            updateMouseButtonState(event.button.button, false);
            break;
        }
        if (event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            mouseWheelX += event.wheel.x;
            mouseWheelY += event.wheel.y;
        }
    }
    float x, y = 0.0f;
    SDL_GetMouseState(&x, &y);
    mouseX = x;
    mouseY = y;

}

BUTTONSTATE InputManager::GetKeyState(SDL_Keycode buttonID)
{
    // Serial.println("checking input");
    return keyboardStates.at(buttonID);
}

void InputManager::updateKeyButtonState(int id, bool pressed)
{
    // check if key id exists in the map
    if (keyboardStates.find(id) == keyboardStates.end())
    {
        // create a new button state if it does not exist
        keyboardStates[id] = BUTTONSTATE::RELEASED; // Default state for new keys
        // return; // Key not found, do nothing
    }
    BUTTONSTATE &state = keyboardStates.at(id); // Get the state reference for the key
    if (pressed)
    {
        state = (state == RELEASED || state == JUSTRELEASED) ? JUSTPRESSED : HELD;
    }
    else
    {
        state = (state == HELD || state == JUSTPRESSED) ? JUSTRELEASED : RELEASED;
    }
}
void InputManager::updateMouseButtonState(int id, bool pressed)
{
    // check if key id exists in the map
    if (mouseButtonStates.find(id) == mouseButtonStates.end())
    {
        // create a new button state if it does not exist
        mouseButtonStates[id] = BUTTONSTATE::RELEASED; // Default state for new mouse buttons
        // return; // Key not found, do nothing
    }
    BUTTONSTATE &state = mouseButtonStates.at(id); // Get the state reference for the key

    if (pressed)
    {
        state = (state == RELEASED || state == JUSTRELEASED) ? JUSTPRESSED : HELD;
    }
    else
    {
        state = (state == HELD || state == JUSTPRESSED) ? JUSTRELEASED : RELEASED;
    }
}
