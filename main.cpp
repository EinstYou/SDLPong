#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window* window;
SDL_Renderer* renderer;


int width;
int height;

bool running;
double deltaTime;

float GetAxis(SDL_Event *e, SDL_Scancode negative, SDL_Scancode positive);

class Vector2 {
public:
    float x;
    float y;
    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

class GameObject {
    private:
    SDL_FRect rect;
    public:
    float velocity;
    GameObject(Vector2 position, Vector2 scale) {
        rect.x = position.x;
        rect.y = position.y;
        rect.w = scale.x;
        rect.h = scale.y;
    }
    void RenderPlayer(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRect(renderer, &rect);
    }
    void Move(Vector2 v) {
        rect.x += v.x;
        rect.y += v.y;
    }
    void SetPosition(Vector2 position) {
        rect.x = position.x;
        rect.y = position.y;
    }
    SDL_FRect GetRect() {
        return rect;
    }
};

class BoxCollision2D {
public:
    static bool IsColliding(GameObject p1, GameObject p2) {
        float p1Bottom = p1.GetRect().y + p1.GetRect().h;
        float p1Top = p1.GetRect().y;
        float p1Left = p1.GetRect().x;
        float p1Right = p1.GetRect().x + p1.GetRect().w;

        float p2Bottom = p2.GetRect().y + p2.GetRect().h;
        float p2Top = p2.GetRect().y;
        float p2Left = p2.GetRect().x;
        float p2Right = p2.GetRect().x + p2.GetRect().w;
        return p1Bottom >= p2Top && p1Top <= p2Bottom && p1Left <= p2Right && p1Right >= p2Left;
    }
};


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

    GameObject p1 = GameObject(Vector2(100, 175 ), Vector2(20, 100));
    float direction;
    float speed = 500;



    Uint64 currentFrame = SDL_GetPerformanceCounter();
    running = true;
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

        p1.velocity = speed * direction;
        p1.Move(Vector2(0.0f, p1.velocity * deltaTime));


        //Rendering
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        p1.RenderPlayer(200,0,0,255);

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