#pragma once

#include "ECS.h"
#include "../Components/Components.h"
#include "Net/Protocol.h"

#ifndef SERVER_BUILD
#include "SpriteComponent.h"
#endif

class InputControllerComponent : public Component {
public:
    TransformComponent* transform;
    #ifndef SERVER_BUILD
    SpriteComponent* sprite;
    #endif

    InputState currentInput;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        #ifndef SERVER_BUILD
        sprite = &entity->getComponent<SpriteComponent>();
        #endif
    }

    void applyInput(const InputState& input) {
        currentInput = input;
    }

    void update() override {
        transform->velocity = {0, 0};

        if (currentInput.up)    transform->velocity.y = -1;
        if (currentInput.down)  transform->velocity.y = 1;
        if (currentInput.left)  { transform->velocity.x = -1; sprite->spriteFlip = SDL_FLIP_HORIZONTAL; }
        if (currentInput.right) { transform->velocity.x = 1;  sprite->spriteFlip = SDL_FLIP_NONE; }

        // if (currentInput.up || currentInput.down || currentInput.left || currentInput.right)
        //     sprite->Play("Walk");
        // else
        //     sprite->Play("Idle");

        #ifndef SERVER_BUILD
        if (currentInput.up || currentInput.down || currentInput.left || currentInput.right)
            sprite->Play("Walk");
        else
            sprite->Play("Idle");
        #endif

        // if (currentInput.shoot) {
        //     float proj_x, proj_y, proj_velx;
        //     if (transform->velocity.x >= 0) {
        //         proj_velx = transform->velocity.x + 0.5f;
        //         proj_x = transform->position.x + transform->width;
        //     } else {
        //         proj_velx = transform->velocity.x - 0.5f;
        //         proj_x = transform->position.x - transform->width;
        //     }
        //     proj_y = transform->position.y;

        //     Game::assets->CreateProjectile(
        //         Vector2D(proj_x, proj_y), Vector2D(proj_velx, 0.0f), 15, "projectile");
        // }
        #ifndef SERVER_BUILD
        if (currentInput.shoot) {
            float proj_x, proj_y, proj_velx;
            if (transform->velocity.x >= 0) {
                proj_velx = transform->velocity.x + 0.5f;
                proj_x = transform->position.x + transform->width;
            } else {
                proj_velx = transform->velocity.x - 0.5f;
                proj_x = transform->position.x - transform->width;
            }
            proj_y = transform->position.y;

            Game::assets->CreateProjectile(
                Vector2D(proj_x, proj_y),
                Vector2D(proj_velx, 0.0f),
                15,
                "projectile"
            );
        }
        #endif
    }
};
