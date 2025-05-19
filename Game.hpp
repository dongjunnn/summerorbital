#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game {

public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    
    void handleEvents();
    void handleKeys();
    void update(uint64_t dt);
    void render(uint64_t dt, uint64_t acc);
    void clean();

    

    bool running() { return isRunning; }

    static SDL_Renderer *renderer;

private:
    bool isRunning;
    SDL_Window *window;
};


#endif