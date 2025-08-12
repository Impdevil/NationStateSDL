#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <unordered_map>

enum BUTTONSTATE
{
    JUSTPRESSED,
    HELD,
    JUSTRELEASED,
    RELEASED
};
enum SCROLLDIRECTION
{
    SCROLL_NONE,
    SCROLL_UP,
    SCROLL_DOWN,
};
struct moueseInfo
{
    float x, y; // Mouse position
    int wheelX, wheelY; // Mouse wheel position
    std::unordered_map<Uint8, BUTTONSTATE> mouseButtonStates;
    bool justpressedLock = false; // Lock for just pressed state
    SCROLLDIRECTION scrollDirY = SCROLL_NONE; // Scroll direction
    SCROLLDIRECTION scrollDirX = SCROLL_NONE; // Scroll direction

};
class InputManager
{
private:
    static InputManager *instance;
    // std::unordered_map<SDL_Keycode,buttonInfo> buttonStates;
    std::unordered_map<SDL_Keycode, BUTTONSTATE> keyboardStates;
    std::unordered_map<Uint8, BUTTONSTATE> mouseButtonStates;
    std::unordered_map<int, BUTTONSTATE> activeFingers;
    float mouseX = 0, mouseY = 0; // Mouse position
    int mouseWheelX = 0, mouseWheelY = 0; // Mouse wheel position
    SCROLLDIRECTION scrollDirY = SCROLL_NONE; // Scroll direction
    SCROLLDIRECTION scrollDirX = SCROLL_NONE; // Scroll direction

    void updateKeyButtonState(int id, bool pressed);
    void updateMouseButtonState(int id,bool pressed);
public:
    bool Running = true; // Flag to control the main loop
    static InputManager *GetInstance();
    void initButton();
    InputManager();
    void ProcessInput();
    BUTTONSTATE GetKeyState(SDL_Keycode buttonID);
    moueseInfo GetMouseInfo() const
    {
        return {mouseX, mouseY, mouseWheelX, mouseWheelY, mouseButtonStates, false,scrollDirY, scrollDirX};
    }
};

#endif