#include "RenderSystem.h"
#include "../Components/Components.h"
#include "../TextureManager.h"


RenderSystem::RenderSystem(SceneInstance& sceneRef) : scene(sceneRef)
{ }

void RenderSystem::render(SDL_Renderer* renderer)
{
	auto& view = scene.GetView<TransformComponent, SpriteComponent>();

	for (Entity e : view)
	{
		auto& t = scene.GetEntityData<TransformComponent>(e);
		auto& s = scene.GetEntityData<SpriteComponent>(e);

		if (scene.HasComponent<RotationComponent>(e))	// yes this is hard coded for now
		{
			// SDL_RendererFlip& animFlip = scene.GetEntityData<AnimationComponent>(e).spriteFlip;
			RotationComponent& rot =  scene.GetEntityData<RotationComponent>(e);

			s.dstRect.x = t.position.x;
			s.dstRect.y = t.position.y;

			SDL_RenderCopyEx(renderer, s.texture, &s.srcRect, &s.dstRect, ((double) rot.angle) * (180/3.1415), NULL, SDL_FLIP_NONE);
			continue;
		}
		else
		{
			s.dstRect.x = t.position.x;
			s.dstRect.y = t.position.y;

			SDL_RenderCopy(renderer, s.texture, &s.srcRect, &s.dstRect);
		}
	}
}

void RenderSystem::renderText(SDL_Renderer* renderer)
{
	auto& view = scene.GetView<LabelComponent>();

	for (Entity e : view)
	{
		auto& label = scene.GetEntityData<LabelComponent>(e);
		std::string& text = label.text;
		TTF_Font* font = label.font;
		SDL_Color colour = label.colour;

		auto& spr = scene.GetEntityData<SpriteComponent>(e).dstRect;

		if (label.toUpdate)		// if there is a change, re render text
		{
			if (label.cache)	// here the cache is outdated, so we get rid of it first
			{
				SDL_DestroyTexture(label.cache);
			}

			SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
			if (!textSurface && !text.empty()) {
				std::cerr << "Error creating label surface for entity " << e << " | " << TTF_GetError() << std::endl;
				return;
			}

			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			if (!textTexture && !text.empty()) {
				std::cerr << "Error creating label texture for entity " << e << " | " << SDL_GetError() << std::endl;
				SDL_FreeSurface(textSurface);
				return;
			}

			label.toUpdate = false;
			label.cache = textTexture;
			// std::cout << "text updated" << std::endl;

			SDL_FreeSurface(textSurface);
		}

		if (!label.cache) continue;		// another lazy guard clause

		int textWidth, textHeight;
		SDL_QueryTexture(label.cache, nullptr, nullptr, &textWidth, &textHeight);

		SDL_Rect dstRect = { spr.x + label.offset_x, spr.y + label.offset_y, textWidth, textHeight };
		SDL_RenderCopy(renderer, label.cache, nullptr, &dstRect);
	}
}
