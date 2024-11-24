#include <SDL2/SDL.h>
#include <iostream>
#include "RectangleComponent.h"
#undef main

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Clean Component Structure", 100, 100, 680, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Component: Bird
    Component bird;
    bird.addComponent<TransformComponent>(70.0f, 60.0f, 50.0f, 50.0f);
    bird.addComponent<RectangleComponent>(SDL_Color{ 255, 255, 0, 255 });

    // Component: Pipe 1
    Component pipe1;
    pipe1.addComponent<TransformComponent>(700.0f, 400.0f, 50.0f, 150.0f);
    pipe1.addComponent<RectangleComponent>(SDL_Color{ 0, 255, 0, 255 });

    // Component: Pipe 2
    Component pipe2;
    pipe2.addComponent<TransformComponent>(700.0f, 0.0f, 50.0f, 150.0f);
    pipe2.addComponent<RectangleComponent>(SDL_Color{ 0, 255, 0, 255 }); 

    float gravity = 200.0f;
    float velocity = 0.0f;

    bool isKeyPressed = false;

    Uint32 lastTime = SDL_GetTicks();

    bool running = true;
    SDL_Event event;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && isKeyPressed == false) {
                velocity = -150.0f;
                isKeyPressed = true;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                isKeyPressed = false;
            }
        }

        // Kuþun hareketi
        velocity += gravity * deltaTime;
        auto birdTransform = bird.getComponent<TransformComponent>();
        birdTransform->y += velocity * deltaTime;

        // Borularýn hareketi
        auto pipe1Transform = pipe1.getComponent<TransformComponent>();
        auto pipe2Transform = pipe2.getComponent<TransformComponent>();
        pipe1Transform->x -= 250.0f * deltaTime;
        pipe2Transform->x -= 250.0f * deltaTime;

        float random = (rand() % 301) - 150;
        if (pipe1Transform->x + pipe1Transform->width < 0) {
            pipe1Transform->x = 680.0f;
            pipe1Transform->y = random + 400.0f;
        }
        if (pipe2Transform->x + pipe2Transform->width < 0) {
            pipe2Transform->x = 680.0f;
            pipe2Transform->y = random;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bird.update();
        bird.render(renderer);

        pipe1.update();
        pipe1.render(renderer);

        pipe2.update();
        pipe2.render(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
