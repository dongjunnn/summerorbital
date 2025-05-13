#include "Game.hpp"

SDL_Texture *chitoTexture;
SDL_Rect srcR, destR;

float x_pos; float y_pos;
float x_vel = 300; float y_vel = 300;
float x_acc = 0; float y_acc = 0;

int WIDTH; int HEIGHT;

Game::Game()
{}
Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
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

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer Created" << std::endl;
        } else {std::cout << "error creating renderer %s\n" << SDL_GetError() << std::endl;}
        isRunning = true;
    }  
    else { isRunning = false; }

    // Create surface from asset, create texture from surface, then free surface now that tex is generated
    SDL_Surface *tmpSurface = IMG_Load("res/canvas.png");
    if (!tmpSurface) {
        printf("error creating surface %s\n", SDL_GetError());
    }
    chitoTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    SDL_QueryTexture(chitoTexture, NULL, NULL, &destR.w, &destR.h);
    SDL_GetWindowSize(window, &width, &height);

    WIDTH = width;
    HEIGHT = height;

    x_pos = (width - destR.w) /2;
    y_pos = (height - destR.h)/2;
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_SPACE:
                    y_acc = 100;
                    break;
                }
                break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_SPACE:
                y_acc = 0;
                y_vel = 300;
                break;
            }
            break;
    }
}

void Game::update()
{   
// window bound collision detection
    if (x_pos <= 0) {
            x_pos = 0;
            x_vel = -x_vel;
        }
    if (y_pos <= 0) {
            y_pos = 0;
            y_vel = -y_vel;
        }
    if (x_pos >= WIDTH - destR.w){
            x_pos = WIDTH - destR.w;
            x_vel = -x_vel;
        }
    if (y_pos >= HEIGHT - destR.h){
            y_pos = HEIGHT - destR.h;
            y_vel = -y_vel;
        }
    
    // calculate new y velocity
    y_vel += y_acc;
    
    // calculate position
    x_pos += x_vel/60;
    y_pos += y_vel/60;

    // update sprite position
    destR.x = (int) x_pos;
    destR.y = (int) y_pos;
    
    // wait 1/60th of a second
    SDL_Delay(1000/60);
}

void Game::render()
{   
    // double buffered
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, chitoTexture, NULL, &destR); // third, fourth args take SDL_Rect to locate asset and destination coords resp.
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Cleaned" << std::endl;
}