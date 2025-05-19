#include "TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    SDL_Surface* tmpSurface = IMG_Load(texture);
    if (!tmpSurface) {
        std::cout << "error creating renderer %s\n" << SDL_GetError() << std::endl;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    if (!tex) {
        std::cout << "error creating texture %s\n" << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(tmpSurface);

    return tex;
}
