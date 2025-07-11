#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100;  // delay in frames in ms

public:

	int animIndex = 0; 

    std::map<const char*, Animation> animations; 

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE; // for flipping the sprite

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {
        setTex(id);
    }

    SpriteComponent(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 8, 100); 

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");

        setTex(id);
    }

    ~SpriteComponent()
    {
    }

    void setTex(std::string id) 
    {   
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

		srcRect.y = animIndex * transform->height; // if animIndex is 0, then srcRect.y = 0, so it will start at the top of the sprite sheet

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform -> width; 
        srcRect.h = transform -> height;
        
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);    // this is tied to framerate, taking note for when 
            srcRect.y = animIndex * srcRect.h;                                                                      // game tick is implemented 
        }
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform -> width * transform -> scale;
        destRect.h = transform -> height * transform -> scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char* animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }

};