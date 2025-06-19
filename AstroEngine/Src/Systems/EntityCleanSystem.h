#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

class EntityCleanSystem
{
public:
	EntityCleanSystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	void cleanProjectiles()
	{
		toDelete.clear();	// clears previous tick's list of entities slated for deletion

		const int screenWidth = 800;
		const int screenHeight = 640;

		auto& projectiles = scene.GetView<ProjectileComponent>();

		for (Entity e : projectiles)
		{
			TransformComponent& tfm = scene.GetEntityData<TransformComponent>(e);
			SpriteComponent& spr = scene.GetEntityData<SpriteComponent>(e);

			int x = static_cast<int>(tfm.position.x);
			int y = static_cast<int>(tfm.position.y);

			if (x < 0 || x + spr.dstRect.w > screenWidth ||
				y < 0 || y + spr.dstRect.h > screenHeight)
			{
				toDelete.push_back(e);
			}
		}

		for (Entity e : toDelete)
		{
			scene.DestroyEntity(e);
			std::cout << "Destroyed entity " << e << ", Projectile Count: " << projectiles.size() << std::endl;
		}
	}

private:
	SceneInstance& scene;
	std::vector<Entity> toDelete;
};