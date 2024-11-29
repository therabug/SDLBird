#pragma once
#include "Component.h"

class UpdatableComponent : public Component {
public:
    virtual void update(float deltaTime) = 0;
};