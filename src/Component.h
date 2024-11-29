#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <stdexcept>  // For exception safety

class Component {
public:
    std::vector<std::unique_ptr<Component>> components;
    Component* parent = nullptr;

    // Add a component to the current component
    template <typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->parent = this;  // Set the parent pointer directly

        T* componentPtr = component.get();
        components.push_back(std::move(component));  // Transfer ownership to components vector

        return componentPtr;
    }

    // Get a component of type T
    template <typename T>
    T* getComponent() {
        // Search in the immediate components
        for (auto& component : components) {
            if (auto ptr = dynamic_cast<T*>(component.get())) {
                return ptr;
            }
        }

        // Recursively check sub-components if not found
        for (auto& component : components) {
            if (auto result = component->getComponent<T>()) {
                return result;
            }
        }

        throw std::runtime_error("Component of type " + std::string(typeid(T).name()) + " not found.");

        return nullptr; // Return nullptr to indicate the component wasn't found
    }

    virtual ~Component() = default;
};