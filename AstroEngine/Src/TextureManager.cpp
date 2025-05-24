#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    if (!tempSurface) { std::cerr << "Failed to load image to surface! SDL_Error:" << SDL_GetError() << std::endl;}

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    if (!tex) {
        std::cerr << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}