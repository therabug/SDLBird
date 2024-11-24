#pragma once
#include "Component.h"
#include <SDL2/SDL.h>
#include "TransformComponent.h"

class RectangleComponent : public Component {
    SDL_Color color;

public:
    RectangleComponent(SDL_Color color) : color(color) {
        if (!parent->getComponent<TransformComponent>()) {
            throw std::runtime_error("RectangleComponent requ+ires a TransformComponent.");
        }
    }

    void render(SDL_Renderer* renderer) override {
        // Get the TransformComponent
        auto transform = parent->getComponent<TransformComponent>();
        if (!transform) return;

        // Draw 
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