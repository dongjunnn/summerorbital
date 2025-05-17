// g++ -std=c++17 main.cpp -o prog -lSDL2 -ldl

// C++ Standard Library
#include<iostream>

// Third-party library
#include<SDL2/SDL.h>
// Include GLAD 
// #include<glad/glad.h>

void SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b) {

  SDL_LockSurface(surface);
  std::cout << "left mouse was pressed (" << x << "," << y << ")\n";
  uint8_t* pixelArray = (uint8_t*)surface->pixels;
  pixelArray[y*surface->pitch + x*surface->format->BytesPerPixel+0] = g;
  pixelArray[y*surface->pitch + x*surface->format->BytesPerPixel+1] = b;
  pixelArray[y*surface->pitch + x*surface->format->BytesPerPixel+2] = r;
  SDL_UnlockSurface(surface);
}

int main(int argc, char* argv[]) {
  
  // Create a window data type
  SDL_Window* window=nullptr;

  // Grab the window surface
  SDL_Surface* screen;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not be initialised: " <<
      SDL_GetError();

  } else {
    std::cout << "SDL video system is ready to go.\n";
  }
  // Before creating window, specify OpenGL version
  /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);*/

  window = SDL_CreateWindow("Spaced Out",
      0,
      0,
      640,
      480,
      SDL_WINDOW_SHOWN);

  screen = SDL_GetWindowSurface(window);

  SDL_Surface* image;
  image = SDL_LoadBMP("../image/space_background.bmp");
  if (!image) {
    std::cerr << "SDL_LoadBMP failed: " << SDL_GetError() << "\n";
    // bail out or at least skip the blit
    return 1;
  }  
  SDL_BlitSurface(image, NULL, screen, NULL);
  SDL_FreeSurface(image);
  SDL_UpdateWindowSurface(window);

  // OpenGL setup the graphics context 
  /*SDL_GLContext context;
  context = SDL_GL_CreateContext(window);*/

  // setup function pointers
  //gladLoadGLLoader(SDL_GL_GetProcAddress);
  
  bool gameIsRunning = true;
  while (gameIsRunning) {
    SDL_Event event;

    int x, y; 
    Uint32 buttons;
    buttons = SDL_GetMouseState(&x, &y);
    // start event loop
    while (SDL_PollEvent(&event)) {
      // glViewport(0, 0, 640, 480);

      if (event.type == SDL_QUIT) {
        gameIsRunning = false;
      }
      if (event.button.button == SDL_BUTTON_LEFT) {
        SetPixel(screen, x, y, 255, 0, 0);
      }
      if (event.button.button == SDL_BUTTON_RIGHT) {
        SetPixel(screen, x, y, 0, 0, 255);
      }
      if (event.type == SDL_MOUSEMOTION) {
        std::cout << "mouse has been moved\n";
      }
      if (event.type == SDL_KEYDOWN) {
        std::cout << "a key has been pressed\n";
        if (event.key.keysym.sym == SDLK_0) {
          std::cout << "0 was pressed\n";
        } else {
          std::cout << "0 was not pressed\n";
        }
      }
      const Uint8* state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_RIGHT]) {
        std::cout << "right arrow key is pressed\n";
      }
      SDL_UpdateWindowSurface(window);
    }
    /*glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);*/
  }


  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;

}
