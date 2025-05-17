/*#include "SDL2/SDL.h"
#include <iostream>

int main() {

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);

  SDL_Delay(3000);

}*/

#include "Game.hpp"

Game* game = nullptr; 

int main(int argc, const char* argv[]) {

  game = new Game();

  game -> init("space engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

  while (game -> running()) {

    game -> handleEvents();
    game -> update();
    game -> render();

  }

  game -> clean();
  return 0;
}
