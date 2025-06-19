#include "TextureManager.h"
#include <SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* renderer)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    if (!tempSurface) { std::cerr << "Failed to load image to surface! SDL_Error:" << SDL_GetError() << std::endl;}

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if (!tex) {
        std::cerr << "Failed to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, tex, &src, &dest, NULL, NULL, flip);
}