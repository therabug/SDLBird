#pragma once
#include "Component.h"
#include <SDL2/SDL.h>

class RenderableComponent : public Component {
public:
    virtual void render(SDL_Renderer* renderer) = 0;
};