#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
    TransformComponent *transform;
	SpriteComponent* sprite;
    RotationComponent* rotation;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
        rotation = &entity->getComponent<RotationComponent>();
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
            case SDLK_SPACE: {
                float proj_x = transform->position.x + transform->width / 2.0f;
                float proj_y = transform->position.y + transform->height / 2.0f;

                float angleRad = rotation->angle * (M_PI / 180.0f);
                float speed = 1.5f;
                float vx = cos(angleRad) * speed;
                float vy = sin(angleRad) * speed;

                float offsetDist = 10.0f;
                Vector2D offset = Vector2D(cos(angleRad), sin(angleRad)) * offsetDist;
                Vector2D spawnPos = Vector2D(proj_x, proj_y) + offset;

                Game::assets->CreateProjectile(spawnPos, Vector2D(vx, vy), 15, "projectile");
                break;
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