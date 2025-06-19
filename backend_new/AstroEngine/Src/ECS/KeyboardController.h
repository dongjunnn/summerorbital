#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
    TransformComponent *transform;
	SpriteComponent* sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
    }


    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = -1;
				sprite->Play("Walk");
                break;
            case SDLK_a:
                transform->velocity.x = -1;
                sprite->Play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL; // flip sprite to the left
                break;
            case SDLK_d:
                transform->velocity.x = 1;
                sprite->Play("Walk");
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                sprite->Play("Walk");
                break;
            default:
                break;
            }
        }
        if (Game::event.type == SDL_KEYDOWN && Game::event.key.repeat == 0)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_SPACE:
                float proj_x;
                float proj_y;
                float proj_velx;
                if (transform->velocity.x >= 0)
                { 
                    proj_velx = transform->velocity.x + 0.5f; 
                    proj_x = transform->position.x + transform->width;
                    proj_y = transform->position.y;
                }
                else 
                { 
                    proj_velx = transform->velocity.x - 0.5f; 
                    proj_x = transform->position.x - transform->width;
                    proj_y = transform->position.y;
                }
               
                Game::assets->CreateProjectile(Vector2D(proj_x, proj_y), Vector2D(proj_velx, 0.0f), 15, "projectile");
            }
        }
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                sprite->Play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            default:
                break;
            }
        }
    }

};