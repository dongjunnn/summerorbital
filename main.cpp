#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int width = 800, height = 600;
const int SCROLL_SPEED = 300;

int main(int argc, char *argv[]) {
    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "HELLO WORLD",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE 
    );   

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create: %s\n", SDL_GetError());
        return 1;
    }
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, render_flags);
    if (!rend) {
        printf("error creating renderer %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load("res/canvas.png");
    if (!surface) {
        printf("error creating surface %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // struct to hold sprite pos and size
    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = (width - dest.w) /2;

    // float precision required for y pos
    float y_pos = height;

    // animation loop
    while (dest.y >= -dest.h)
    {
        // clear the window
        SDL_RenderClear(rend);

        // set the y position in the struct
        dest.y = (int) y_pos;
    
        // draw the image to the window
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        // update sprite position
        y_pos -= (float) SCROLL_SPEED / 60;

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window); 
    SDL_Quit();
    return 0;
}