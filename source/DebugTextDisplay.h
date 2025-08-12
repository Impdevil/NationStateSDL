#ifndef DEBUGTEXTDISPLAY_H
#define DEBUGTEXTDISPLAY_H
#include <functional>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextDisplay.h"
#include <type_traits>
#include <string>
#include "BaseObjects.h"
#include <any>
#include <variant>

using DebugReturnType = std::variant<std::string, SDL_FRect, SDL_FPoint>;
struct propertyHolder
{
    std::string displayText;
    std::function<DebugReturnType()> callback;
    std::string createString()
    {
        DebugReturnType val = callback();

        std::string builtString = std::visit([](const auto &val) -> std::string
                                             {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, std::string>)
                return ": " + val;
            else if constexpr (std::is_same_v<T, SDL_FRect>)
            {
                return Utils::to_string(val);
            }
            else if constexpr(std::is_same_v<T, SDL_FPoint>)
            {
                return Utils::to_string(val);
            }
            else
            {
                return " unable to set.";
            } }, val);

        return "" + displayText +" "+ builtString;
    }
};

class DebugsDisplay : public renderableObject
{
public:
    virtual ~DebugsDisplay();
    virtual void updateProperties() =0;
    virtual propertyHolder addPropertyString(std::string displayText, std::function<std::string()> Callback);
    virtual propertyHolder addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback);
    virtual propertyHolder addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback);
};
/// @brief used for in world display of text on scene
class DebugTextSingleLine : public DebugsDisplay
{
private:
    TextDisplay textDisplay;
    propertyHolder properties;

public:
    DebugTextSingleLine() = default;
    DebugTextSingleLine(SDL_Renderer *ren,bool worldSpaceDisplay ,float x, float y, TTF_Font *debugFont);
    ~DebugTextSingleLine();

    propertyHolder addPropertyString(std::string displayText, std::function<std::string()> Callback) override;
    propertyHolder addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback) override;
    propertyHolder addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback) override;
    void updateProperties() override;
    void render(SDL_Renderer *renderer) const override;
};


/// @brief used to create a single line of text on the screen.
class DebugTextMultiLine : public DebugsDisplay
{
private:

    TextDisplay textDisplay;
    std::vector<propertyHolder> properties;
public:
    DebugTextMultiLine() = default;
    DebugTextMultiLine(SDL_Renderer *ren, bool worldSpaceDisplay ,float x, float y, TTF_Font *debugFont);
    ~DebugTextMultiLine();
    propertyHolder addPropertyString(std::string displayText, std::function<std::string()> Callback) override;
    propertyHolder addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback) override;
    propertyHolder addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback) override;
    void updateProperties() override;
    void render(SDL_Renderer *renderer) const override;
};

#endif // DEBUGTEXTDISPLAY_H
