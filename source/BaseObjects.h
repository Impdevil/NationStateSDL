#ifndef baseObjects_h
#define baseObjects_h

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class renderableObject{
public:
    virtual void render(SDL_Renderer *renderer) const = 0; // Pure virtual function for rendering
    virtual ~renderableObject() = default; // Virtual destructor for proper cleanup
};


#endif // baseObjects_h