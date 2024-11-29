#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "RectangleComponent.h"
#include "TransformComponent.h"
#include "UpdateSystem.h"
#include "RenderSystem.h"
#include "Component.h"
#undef main

class CameraComponent : public Component {
public:
    float x, y, width, height;
    CameraComponent(float x = 0.0f, float y = 0.0f, float width = 680.0f, float height = 480.0f)
        : x(x), y(y), width(width), height(height) {
    }
};

class CameraSystem {
public:
    void update(Component* camera, Component* target) {
        auto cameraTransform = camera->getComponent<CameraComponent>();
        auto targetTransform = target->getComponent<TransformComponent>();

        if (cameraTransform && targetTransform) {
            cameraTransform->x = targetTransform->x - cameraTransform->width / 2 + targetTransform->width / 2;
            cameraTransform->y = targetTransform->y - cameraTransform->height / 2 + targetTransform->height / 2;
        }
    }
};


class VelocityComponent : public Component {
public:
    float vx, vy;
    VelocityComponent(float vx = 0.0f, float vy = 0.0f) : vx(vx), vy(vy) {}
};

class CollisionSystem {
public:
    void checkCollisions(Component* player, const std::vector<Component*>& platforms) {
        auto playerTransform = player->getComponent<TransformComponent>();
        auto playerVelocity = player->getComponent<VelocityComponent>();
        if (!playerTransform || !playerVelocity) return;


        for (auto& platform : platforms) {
            auto platformTransform = platform->getComponent<TransformComponent>();
            if (platformTransform && checkCollision(playerTransform, platformTransform)) {
                if (playerTransform->y > 0) { // Sadece aþaðý doðru hareket ederken zýplamayý durdur
                    playerVelocity->vy = 0;
                    playerTransform->y = platformTransform->y - playerTransform->height;
                }
            }
        }
    }

private:
    bool checkCollision(TransformComponent* a, TransformComponent* b) {
        float leftA = a->x;
        float rightA = a->x + a->width;
        float topA = a->y;
        float bottomA = a->y + a->height;

        float leftB = b->x;
        float rightB = b->x + b->width;
        float topB = b->y;
        float bottomB = b->y + b->height;

        if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB) {
            return false;
        }
        return true;
    }
};


int main(int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mini Platformer", 100, 100, 680, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Component: Player
    Component player;
    player.addComponent<TransformComponent>(70.0f, 350.0f, 50.0f, 50.0f);
    player.addComponent<RectangleComponent>(SDL_Color{ 255, 0, 0, 255 });
    player.addComponent<VelocityComponent>();

    // Component: Platform
    std::vector<Component> platforms(2);
    platforms[0].addComponent<TransformComponent>(50.0f, 400.0f, 600.0f, 50.0f);
    platforms[0].addComponent<RectangleComponent>(SDL_Color{ 0, 255, 0, 255 });

    platforms[1].addComponent<TransformComponent>(200.0f, 300.0f, 200.0f, 20.0f);
    platforms[1].addComponent<RectangleComponent>(SDL_Color{ 0, 255, 0, 255 });

    // Component: Camera
    Component camera;
    camera.addComponent<CameraComponent>();

    float gravity = 889.0f;
    bool isKeyPressed = false;

    Uint32 lastTime = SDL_GetTicks();

    bool running = true;
    SDL_Event event;

    UpdateSystem updateSystem;
    RenderSystem renderSystem;
    CollisionSystem collisionSystem;
    CameraSystem cameraSystem;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            auto playerVelocity = player.getComponent<VelocityComponent>();
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP && !isKeyPressed) {
                    playerVelocity->vy = -300.0f;  // Jump velocity
                    isKeyPressed = true;
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    playerVelocity->vx = -200.0f;  // Move left
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    playerVelocity->vx = 200.0f;  // Move right
                }
            }
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_UP) {
                    isKeyPressed = false;
                }
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                    playerVelocity->vx = 0.0f;  // Stop horizontal movement
                }
            }
        }

        auto playerTransform = player.getComponent<TransformComponent>();
        auto playerVelocity = player.getComponent<VelocityComponent>();
        playerVelocity->vy += gravity * deltaTime;
        playerTransform->x += playerVelocity->vx * deltaTime;
        playerTransform->y += playerVelocity->vy * deltaTime;

        // Check collisions with platforms
        std::vector<Component*> platformPtrs;
        for (auto& platform : platforms) platformPtrs.push_back(&platform);
        collisionSystem.checkCollisions(&player, platformPtrs);

        // Update camera position
        cameraSystem.update(&camera, &player);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render player
        auto cameraTransform = camera.getComponent<CameraComponent>();
        render(&player, renderer, cameraTransform);

        // Render platforms
        for (auto& platform : platforms) {
            render(&platform, renderer, cameraTransform);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
