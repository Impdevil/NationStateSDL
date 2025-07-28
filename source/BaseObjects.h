#ifndef baseObjects_h
#define baseObjects_h

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class renderableObject
{
public:
    virtual void render(SDL_Renderer *renderer) const = 0; // Pure virtual function for rendering
    virtual ~renderableObject() = default;                 // Virtual destructor for proper cleanup
};

#include <sstream>
namespace Utils
{

    static std::string to_string(const SDL_FRect &r)
    {
        std::ostringstream oss;
        oss << r.x << ", " << r.y << ", " << r.w << ", " << r.h;
        return oss.str();
    }

    static std::string to_string(const SDL_FPoint &p)
    {
        std::ostringstream oss;
        oss << p.x << ", " << p.y;
        return oss.str();
    }
}

#endif // baseObjects_h