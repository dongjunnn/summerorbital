#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL.h>

class TileComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, dstRect;


	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
	{
		texture = Game::assets->GetTexture(id);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		dstRect.x = xpos;
		dstRect.y = ypos;
		dstRect.w = dstRect.h = tsize * tscale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, dstRect, SDL_FLIP_NONE);
	}

};