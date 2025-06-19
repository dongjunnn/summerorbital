#pragma once

#include "Net/Protocol.h"
#include "ECS.h"
#include "../Game.h"

class KeyboardInputSource {
public:
    static InputState pollInput() {
        InputState input;

        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        input.up    = keystates[SDL_SCANCODE_W];
        input.down  = keystates[SDL_SCANCODE_S];
        input.left  = keystates[SDL_SCANCODE_A];
        input.right = keystates[SDL_SCANCODE_D];

        // SDL_KEYDOWN event for non-continuous keys like spacebar (shoot)
        if (Game::event.type == SDL_KEYDOWN && Game::event.key.repeat == 0) {
            if (Game::event.key.keysym.sym == SDLK_SPACE) {
                input.shoot = true;
            }
        }

        return input;
    }
};