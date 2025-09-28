#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Renderer {
public:
    static void Render() {

    }
};

class GameObject : public Renderer {
public:
    SDL_FRect transform;
    GameObject(SDL_FRect rect) {
        this->transform = rect;
    }

};

SDL_Window* window;
SDL_Renderer* renderer;

int width;
int height;
bool running;

float speed = 500;

double deltaTime;

float direction = 0;

float GetAxis(SDL_Event *e, SDL_Scancode negative, SDL_Scancode positive);

int main(int argc, char *argv[]) {

    width = 800;
    height = 450;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL can't be initialized\n");
        return 1;
    }

    window = SDL_CreateWindow("Pong", width, height, 0);
    if (window == nullptr) {
        printf("SDL_Window can't be created\n");
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        printf("SDL_Renderer can't be created\n");
        return 1;
    }

    running = true;

    SDL_FRect p1;
    p1.h = 100;
    p1.w = 20;
    p1.x = 0 + 20;
    p1.y = (float)(height / 2) - (p1.h / 2);
    float p1VelocityY;

    float p1Bottom = p1.y + p1.h;
    float p1Top = p1.y;



    SDL_FRect p2;
    p2.h = 100;
    p2.w = 20;
    p2.x = width - p2.w - 20;
    p2.y = (float)(height / 2) - (p2.h / 2);
    float p2VelocityY;
    Uint64 currentFrame = SDL_GetPerformanceCounter();

    while (running) {


        //Calculate DeltaTime
        Uint64 nextFrame =  SDL_GetPerformanceCounter();
        deltaTime = (double)(nextFrame - currentFrame) / SDL_GetPerformanceFrequency();
        currentFrame = nextFrame;

        //Handle event
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            direction = GetAxis(&e, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);
        }



        p1Bottom = p1.y + p1.h;
        p1Top = p1.y;

        if (p1Top < 0) {
            p1Top = 0;
            p1.y = p1Top;
        }
        else if (p1Bottom > (float)height) {
            p1Bottom = (float)height;
            p1.y = p1Bottom - p1.h;
        }

        p1VelocityY = direction * speed ;
        p1.y += p1VelocityY * (float)deltaTime;


        Renderer::Render();

        //Rendering
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 0);
        SDL_RenderFillRect(renderer, &p1);

        SDL_SetRenderDrawColor(renderer, 0, 0, 150, 0);
        SDL_RenderFillRect(renderer, &p2);

        SDL_RenderPresent(renderer);
    }



    return 0;
}


float GetAxis(SDL_Event *e, SDL_Scancode negative, SDL_Scancode positive) {
    float result = 0;
    if (e->type == SDL_EVENT_KEY_DOWN) {
        if (e->key.scancode == negative) {
            result -= 1;
        }
        else if (e->key.scancode == positive) {
            result += 1;
        }
    }
    return result;
}