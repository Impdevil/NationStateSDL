#include "DebugTextDisplay.h"

#pragma region DebugDisplay

DebugsDisplay::~DebugsDisplay() = default;

propertyHolder DebugsDisplay::addPropertyString(std::string displayText, std::function<std::string()> Callback)
{
    propertyHolder props = {displayText, Callback};
    return props;
}
propertyHolder DebugsDisplay::addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback)
{
    propertyHolder props = {displayText, Callback};
    return props;
}
propertyHolder DebugsDisplay::addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback)
{
    propertyHolder props = {displayText, Callback};
    return props;
}
#pragma endregion

#pragma region DebugTextSingleLine
DebugTextSingleLine::DebugTextSingleLine(SDL_Renderer *ren, bool worldSpaceDisplay, float x, float y, TTF_Font *debugFont)
    : textDisplay(ren, worldSpaceDisplay, x, y, debugFont, "Null", {0, 0, 0, 0})

{
}
DebugTextSingleLine::~DebugTextSingleLine()
{
    properties = {"", NULL};
}

propertyHolder DebugTextSingleLine::addPropertyString(std::string displayText, std::function<std::string()> Callback)
{
    properties = DebugsDisplay::addPropertyString(displayText, Callback);
    return properties;
}
propertyHolder DebugTextSingleLine::addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback)
{
    properties = DebugsDisplay::addPropertyFRect(displayText, Callback);
    return properties;
}
propertyHolder DebugTextSingleLine::addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback)
{
    properties = DebugsDisplay::addPropertyOrigin(displayText, Callback);
    return properties;
}
void DebugTextSingleLine::updateProperties()
{
    textDisplay.updateLabelText(properties.createString());
}

void DebugTextSingleLine::render(SDL_Renderer *ren) const
{
    textDisplay.render(ren);
}
#pragma endregion

#pragma region DebugTextMultiLine

DebugTextMultiLine::DebugTextMultiLine(SDL_Renderer *ren, bool worldSpaceDisplay, float x, float y, TTF_Font *debugFont)
    : textDisplay(ren, worldSpaceDisplay, x, y, debugFont, "Null", {0, 0, 0, 0})

{
}

DebugTextMultiLine::~DebugTextMultiLine()
{
    properties.clear();
}

propertyHolder DebugTextMultiLine::addPropertyString(std::string displayText, std::function<std::string()> Callback)
{
    propertyHolder prop = DebugsDisplay::addPropertyString(displayText, Callback);
    ;
    properties.emplace_back(prop);
    return prop;
}

propertyHolder DebugTextMultiLine::addPropertyFRect(std::string displayText, std::function<SDL_FRect()> Callback)
{
    propertyHolder prop = DebugsDisplay::addPropertyFRect(displayText, Callback);
    ;
    properties.emplace_back(prop);
    return prop;
}
propertyHolder DebugTextMultiLine::addPropertyOrigin(std::string displayText, std::function<SDL_FPoint()> Callback)
{
    propertyHolder prop = DebugsDisplay::addPropertyOrigin(displayText, Callback);
    ;
    properties.emplace_back(prop);
    return prop;
}

void DebugTextMultiLine::updateProperties()
{
    std::string buildString;
    for (auto &val : properties)
    {
        buildString += val.createString();
        buildString += "\n";
    }
    textDisplay.updateLabelText(buildString);
}

void DebugTextMultiLine::render(SDL_Renderer *ren) const
{
    textDisplay.render(ren);
}