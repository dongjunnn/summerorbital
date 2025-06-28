#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

using AnimData = std::unordered_map<std::string, AnimationData>;

class AnimationSystem
{
public:
	AnimationSystem(SceneInstance& scene) : scene(scene) {}

	void PlayerAnim()
	{
		auto& view = scene.GetView<PlayerComponent, AnimationComponent>();

		for (Entity e : view)
		{
			SDL_RendererFlip& flip = scene.GetEntityData<AnimationComponent>(e).spriteFlip;
			AnimData& ani = scene.GetEntityData<AnimationComponent>(e).animations;
			SDL_Rect& spr = scene.GetEntityData<SpriteComponent>(e).srcRect;
			Vector2D& vel = scene.GetEntityData<VelocityComponent>(e).velocity;
			
			if (vel.x < 0.0f)
			{
				AnimationData& walk = ani["walk"];
				spr.x = spr.w * static_cast<int>((SDL_GetTicks() / walk.speed) % walk.frames);    // this is tied to framerate, taking note for when 
				spr.y = walk.index * spr.h;
				flip = SDL_FLIP_HORIZONTAL;
			}
			else if (vel.x > 0.0f)
			{
				AnimationData& walk = ani["walk"];
				spr.x = spr.w * static_cast<int>((SDL_GetTicks() / walk.speed) % walk.frames);    // this is tied to framerate, taking note for when 
				spr.y = walk.index * spr.h;
				flip = SDL_FLIP_NONE;
			}
			else
			{
				AnimationData& walk_l = ani["idle"];
				spr.x = spr.w * static_cast<int>((SDL_GetTicks() / walk_l.speed) % walk_l.frames);    // this is tied to framerate, taking note for when 
				spr.y = walk_l.index * spr.h;
				flip = SDL_FLIP_NONE;
			}
		}
	}

private:
	SceneInstance& scene;
};
