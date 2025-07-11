#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

class PlayerSystem
{
public:
	PlayerSystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	void handleHP()
	{
		auto& players = scene.GetView<PlayerComponent>();

		for (Entity p : players)
		{
			int& hp = scene.GetEntityData<HealthComponent>(p).hp;
			uint8_t& alpha = scene.GetEntityData<SpriteComponent>(p).alpha;

			if (alpha != 0 && hp <= 0)	// this check is done in UI system too, but this is not its job
			{							// i think they should be separate, what do you think?
				
				alpha = 0;	// so defeated player is transparent, yay
			}
			else if (alpha == 0 && hp > 0)
			{
				alpha = 255;	// so revived player is opaque
			}
		}
	}

private:
	SceneInstance& scene;
};