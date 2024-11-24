#include <SDL2/SDL.h>
#include <iostream>
#undef main

struct Bird {
	float x_pos;
	float y_pos;
	float x_size;
	float y_size;
};

struct Pipe {
	float x_pos;
	float y_pos;
	float x_size;
	float y_size;
};

float gravity = 0.1f;
float velocity = 0.0f;

const float JUMP_POWER = 5.0f;

const float START_X = 70.0f;
const float START_Y = 60.0f;
const float START_SIZE = 50.0f;

Bird bird = { START_X, START_Y, START_SIZE, START_SIZE };
Pipe pipe = { 700.0f, 0.0f, 50.0f, 480.0f };
Pipe pipe2 = { 700.0f, 0.0f, 50.0f, 480.0f };

int r, g, b = 0;

void game_over() {
	bird.x_pos = START_X;
	bird.y_pos = START_Y;
	pipe.x_pos = 700.0f;
	pipe2.x_pos = 700.0f;
	pipe.y_pos = 350.0f;
	pipe2.y_pos = -350.0f;
	velocity = 0.0f;
}

void bird_collides(Pipe pipe) {
	if (bird.x_pos + bird.x_size >= pipe.x_pos && bird.x_pos <= pipe.x_pos + pipe.x_size && bird.y_pos + bird.y_size >= pipe.y_pos && bird.y_pos <= pipe.y_pos + pipe.y_size) {
		game_over();
	}
}

void update_pipes() {
	if (pipe.x_pos < -50.0f) {
		pipe.x_pos = 700.0f;
		float random = (rand() % 150);
		pipe.y_pos = random + 350.0f;
		pipe2.x_pos = 700.0f;
		pipe2.y_pos = random - 350.0f;
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		
	}
	pipe.x_pos -= 2.0f;
	pipe2.x_pos -= 2.0f;
	bird_collides(pipe);
	bird_collides(pipe2);
}

int main(int argc, char *argv[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create a window
	SDL_Window *window = SDL_CreateWindow("Hello SDL",
		100, 100, 680, 480, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		SDL_DestroyWindow(window);
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Rect square = {};
	square.x = bird.x_pos;  // x position
	square.y = bird.y_pos;  // y position
	square.w = bird.x_size;  // width
	square.h = bird.y_size;  // height

	SDL_Rect pipe_square = {};
	pipe.y_pos += 350.0f;
	pipe_square.x = pipe.x_pos;  // x position
	pipe_square.y = pipe.y_pos;  // y position
	pipe_square.w = 50.0f;  // width
	pipe_square.h = 480.0f;  // height

	SDL_Rect pipe_square2 = {};
	pipe2.y_pos -= 350.0f;
	pipe_square2.x = pipe2.x_pos;  // x position
	pipe_square2.y = pipe2.y_pos;  // y position
	pipe_square2.w = 50.0f;  // width
	pipe_square2.h = 480.0f;  // height

	bool key_pressed = false;

	// Main event loop
	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_SPACE && key_pressed == false)
				{
					velocity = -JUMP_POWER;
					key_pressed = true;
				}
			}

			if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_SPACE && key_pressed == true) {
					key_pressed = false;
				}
			}

			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		velocity += gravity;
		bird.y_pos += velocity;
		update_pipes();

		square.x = bird.x_pos;  // x position
		square.y = bird.y_pos;  // y position
		pipe_square.x = pipe.x_pos;  // x position
		pipe_square.y = pipe.y_pos;
		pipe_square2.x = pipe2.x_pos;  // x position
		pipe_square2.y = pipe2.y_pos;


		if (bird.y_pos + bird.y_size >= 480 || bird.y_pos <= 0) {
			game_over();
		}

		// Better coloring
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);

		// Render the square
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &square);

		// Render the pipe square
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &pipe_square);

		// Render the pipe square 2
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &pipe_square2);

		// Present the backbuffer
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}