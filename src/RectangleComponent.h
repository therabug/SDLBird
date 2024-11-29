#pragma once

#include "Component.h"
#include <SDL2/SDL.h>
#include "TransformComponent.h"
#include <iostream>
#include "RenderableComponent.h"

class RectangleComponent : public RenderableComponent {
    

public:
    SDL_Color color;
    RectangleComponent(SDL_Color color) : color(color) {}

    void render(SDL_Renderer* renderer) override {
        // Get the TransformComponent
        auto transform = parent->getComponent<TransformComponent>();
        if (!transform) {
            std::cerr << "Error: TransformComponent missing from parent entity.\n";
            return;
        }

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