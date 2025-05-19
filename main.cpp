#include "Game.hpp"
#include <fstream>

Game *game = nullptr;

int main(int argc, char *argv[]) 
{
    // microstutter?
    
    uint64_t t = 0;
    const uint64_t dt = 10; 
    uint32_t gameTicks = 0;

    uint64_t frameStart = SDL_GetTicks64();
    uint64_t accumulator = 0;

    game = new Game();
    
    game -> init("lolEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    while (game -> running()) 
    {
        uint64_t frameEnd = SDL_GetTicks64();
        uint64_t frameTime = frameEnd - frameStart;

        Uint64 start = SDL_GetPerformanceCounter(); // for frame rate 

        frameStart = frameEnd;
        accumulator += frameTime;

        while (accumulator >= dt) 
        {

        game -> handleEvents();
        game -> handleKeys();
        game -> update(dt);
        gameTicks ++;
        std::cout << gameTicks << " tick" << std::endl;
        t += dt;
        accumulator -= dt;

        }

        game -> render(dt, accumulator);

        // for fps counter
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
        // std::cout << "Current FPS: " << std::to_string(1.0f / elapsed) << std::endl;
    }

    game -> clean();
    return 0;
}