#pragma once
#include "Component.h"
#include <SDL2/SDL.h>
#include "TransformComponent.h"

class RectangleComponent : public Component {
    SDL_Color color;

public:
    RectangleComponent(SDL_Color color) : color(color) {}

    void render(SDL_Renderer* renderer) override {
        // Transform bile�enini al
        auto transform = parent->getComponent<TransformComponent>();
        if (!transform) return;

        // Transform kullan�larak �izim yap�l�r
        SDL_Rect rect = {
            static_cast<int>(transform->x),
            static_cast<int>(transform->y),
            static_cast<int>(transform->width),
            static_cast<int>(transform->height)
        };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
};