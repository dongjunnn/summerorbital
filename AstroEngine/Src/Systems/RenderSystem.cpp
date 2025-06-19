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

		s.dstRect.x = t.position.x;
		s.dstRect.y = t.position.y;
		
		SDL_RenderCopy(renderer, s.texture, &s.srcRect, &s.dstRect);
	}
}