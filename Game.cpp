#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"

SDL_Renderer *Game::renderer = nullptr;

const Uint8 *keyState;  // keyboard state

GameObject* chito;
int HEIGHT; int WIDTH;

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    flags |= SDL_RENDERER_ACCELERATED;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        std::cout << "SDL Initialised" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) 
        {
            std::cout << "Window Created" << std::endl;
        } else {SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create: %s\n", SDL_GetError());}

        // presentvsync is a sad fix, delta time soon
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer Created" << std::endl;
        } else {std::cout << "error creating renderer %s\n" << SDL_GetError() << std::endl;}
        isRunning = true;
    }  
    else { isRunning = false; }

    // Getting window height
    WIDTH = width;
    HEIGHT = height;
    
    // chito
    chito = new GameObject("res/canvas.png", 0, 0);

}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    struct telemetry tmp =  chito->getTelemetry();
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_SPACE:
                    tmp.y_acc = 2000;
                    chito->setTelemetry(&tmp);
                    break;
                }
                break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_SPACE:
                tmp.y_acc = 0;
                tmp.y_vel = 300;
                chito->setTelemetry(&tmp);
                break;
            }
            break;
    }
    // std::cout << "Events";
}

void Game::handleKeys()
{   
    struct telemetry tmp =  chito->getTelemetry();
    keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_A]){  // scancode keeps keyboard key locations across keyboard layouts
        tmp.angle_vel = -120;
        chito->setTelemetry(&tmp);
        // std::cout << tmp.angle_vel << std::endl;
    } else if (keyState[SDL_SCANCODE_D])
    {
        tmp.angle_vel = 120;
        chito->setTelemetry(&tmp);
        // std::cout << tmp.angle_vel << std::endl;
    } else {
        tmp.angle_vel = 0;
        chito->setTelemetry(&tmp);
        // std::cout << tmp.angle_vel << std::endl;
    }
    // std::cout << "Keys" << std::endl;
}

void Game::update(uint64_t dt)
{   
   chito->Update(dt);
}

void Game::render(uint64_t dt, uint64_t acc)
{   
    // double buffered
    SDL_RenderClear(renderer);
    chito->Render(dt, acc);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Cleaned" << std::endl;
}