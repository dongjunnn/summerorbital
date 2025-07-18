#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    if (!objTexture) {
        std::cerr << "GameObject failed to load texture: " << texturesheet << std::endl;
    }

    xpos = x;
    ypos = y;
}

void GameObject::Update()
{
    xpos++;
    ypos++; 

    srcRect.h = 1000;
    srcRect.w = 1000;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w * 2;
    destRect.h = srcRect.h * 2;
}

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}