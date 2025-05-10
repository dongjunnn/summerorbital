// C++ Standard Library
#include<iostream>

// Third-party library
#include<SDL2/SDL.h>

int main(int argc, char* argv[]) {
  
  // Create a window data type
  SDL_Window* window=nullptr;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not be initialised: " <<
      SDL_GetError();

  } else {
    std::cout << "SDL video system is ready to go.\n";
  }


  window = SDL_CreateWindow("Spaced Out",
      0,
      0,
      640,
      480,
      SDL_WINDOW_SHOWN);
  SDL_Delay(3000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;

}
