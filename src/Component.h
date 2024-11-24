#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <stdexcept>  // For exception safety

class Component {
public:
    std::vector<std::unique_ptr<Component>> children;
    Component* parent = nullptr;

    // Add a component to the current component
    template <typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->parent = this;  // Set the parent pointer directly

        T* componentPtr = component.get();
        children.push_back(std::move(component));  // Transfer ownership to children vector

        return componentPtr;
    }

    // Get a component of type T
    template <typename T>
    T* getComponent() {
        for (auto& component : children) {
            if (auto ptr = dynamic_cast<T*>(component.get())) {
                return ptr;
            }
        }

        // Recursively check children components
        for (auto& component : children) {
            if (auto result = component->getComponent<T>()) {
                return result;
            }
        }

        return nullptr;
    }

    // Update the component and all its children recursively
    virtual void update() {
        for (auto& component : children) {
            component->update();
        }
    }

    // Render the component and all its children recursively
    virtual void render(SDL_Renderer* renderer) {
        for (auto& component : children) {
            component->render(renderer);
        }
    }

    // Destructor
    virtual ~Component() = default;
};
