#pragma once
#include "Component.h"

class TransformComponent : public Component {
public:
    float x, y;
    float width, height;

    TransformComponent(float x = 0.0f, float y = 0.0f, float width = 50.0f, float height = 50.0f)
        : x(x), y(y), width(width), height(height) {
    }
};
