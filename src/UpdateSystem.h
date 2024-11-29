#pragma once

#include "UpdatableComponent.h"
#include <vector>

class UpdateSystem {
public:
    void update(Component* root, float deltaTime) {
        for (auto& component : root->components) {
            if (auto updatable = dynamic_cast<UpdatableComponent*>(component.get())) {
                updatable->update(deltaTime);
            }
            update(component.get(), deltaTime);
        }
    }
};