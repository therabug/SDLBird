#pragma once

#include "RenderableComponent.h"
#include <vector>

class RenderSystem {
public:
    void render(Component* root, SDL_Renderer* renderer) {
        for (auto& component : root->components) {
            if (auto renderable = dynamic_cast<RenderableComponent*>(component.get())) {
                renderable->render(renderer);
            }
            render(component.get(), renderer);
        }
    }
};
