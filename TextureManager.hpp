#pragma once // ensures this is included only once

#include "Game.hpp"

class TextureManager {

public:
    static SDL_Texture *LoadTexture(const char *fileName);


};