#include <iostream>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>

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
    void Normalize() {
        float hipo = sqrt(x * x + y * y);
        x /= hipo;
        y /= hipo;
    }
};

class RandomGenerator {
public:
    static int Generate(int maxValue){
        int random = SDL_rand(maxValue + 1);
        int negOrPos = SDL_rand(2);
        if(negOrPos == 1) {
            return random;
        }
        return -random;
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
    void RenderObject(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
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

    GameObject p1 = GameObject(Vector2(20, 175 ), Vector2(20, 100));
    float direction;
    float speed = 500;
    GameObject topWall = GameObject(Vector2(0, -50), Vector2((float)width, 50));
    GameObject bottomWall = GameObject(Vector2(0, (float)height), Vector2((float)width, 50));

    GameObject ball = GameObject(Vector2((float)width / 2 - 10, (float)height / 2 - 10), Vector2(15, 15));
    Vector2 ballDirection = Vector2((float)(RandomGenerator::Generate(50)), (float)(RandomGenerator::Generate(30)));
    ballDirection.Normalize();
    float ballSpeed = 300;
    Vector2 ballVelocity = Vector2(0,0);

    Uint64 currentFrame = SDL_GetPerformanceCounter();
    running = true;
    while (running) {


        //Calculate DeltaTime
        Uint64 nextFrame =  SDL_GetPerformanceCounter();
        deltaTime = (double)(nextFrame - currentFrame) / (double)SDL_GetPerformanceFrequency();
        currentFrame = nextFrame;

        //Handle event
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            direction = GetAxis(&e, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN);
        }

        Vector2 oldPosP1 = Vector2(p1.GetRect().x, p1.GetRect().y);
        p1.velocity = speed * direction;
        p1.Move(Vector2(0.0f, (float)p1.velocity * deltaTime));
        if (BoxCollision2D::IsColliding(p1, bottomWall) || BoxCollision2D::IsColliding(p1, topWall)) {
            p1.SetPosition(oldPosP1);
        }

        Vector2  oldPosBall = Vector2(ball.GetRect().x, ball.GetRect().y);
        ballVelocity = Vector2(ballDirection.x * ballSpeed, ballDirection.y * ballSpeed);
        ball.Move(Vector2((float)ballVelocity.x * deltaTime, (float)ballVelocity.y * deltaTime));

        if (BoxCollision2D::IsColliding(ball, bottomWall) || BoxCollision2D::IsColliding(ball, topWall)) {
            ball.SetPosition(oldPosBall);
            ballDirection.y *= -1;
        }
        else if (BoxCollision2D::IsColliding(ball, p1)) {
            ball.SetPosition(oldPosBall);
            ballDirection.x *= -1;
        }
        //Rendering
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        p1.RenderObject(200,0,0,255);
        ball.RenderObject(255, 255, 0, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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