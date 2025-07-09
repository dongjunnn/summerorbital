#pragma once
#include <SDL.h>

struct SpriteComponent
{
	SDL_Texture* texture;
	SDL_Rect srcRect, dstRect;
	uint8_t alpha = 255;
};
