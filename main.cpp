#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window* window;
SDL_Renderer* renderer;

class Rectangle {
public:
    SDL_FRect rect;
    int r;
    int g;
    int b;
    int a;
    Rectangle(float x, float y, float w, float h, int r, int g, int b, int a) {
        this->rect.x = x;
        this->rect.y = y;
        this->rect.w = w;
        this->rect.h = h;

        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};
static std::vector<Rectangle> rects;
class GameObject {
public:
    static void CreateObject(Rectangle rect) {
        rects.push_back(rect);
    }
    static void RenderObjects() {
        for (auto &rect : rects) {
            SDL_SetRenderDrawColor(renderer, rect.r, rect.g, rect.b, rect.a);
            SDL_RenderFillRect(renderer, &rect.rect);
        }
    }
};



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

    Rectangle p1 = Rectangle(100, 100, 50, 50, 0, 100, 0, 100);
    GameObject::CreateObject(p1);


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





        //Rendering


        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        GameObject::RenderObjects();


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