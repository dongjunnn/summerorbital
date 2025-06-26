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
			SDL_Rect& col = scene.GetEntityData<ColliderComponent>(e).collider;

			int x = static_cast<int>(tfm.position.x);
			int y = static_cast<int>(tfm.position.y);

			if (x < 0 || x + col.w > screenWidth ||
				y < 0 || y + col.h > screenHeight) 
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

	void clearDefeatedPlayers()
	{
		toDelete.clear();

		auto& players = scene.GetView<PlayerComponent>();

		for (Entity player : players)
		{
			int& hp = scene.GetEntityData<HealthComponent>(player).hp;

			if (hp <= 0)
			{
				toDelete.push_back(player);
			}
		}

		for (Entity e : toDelete)
		{
			scene.DestroyEntity(e);
			std::cout << "Destroyed player  " << e << ", Player Count: " << players.size() << std::endl;
		}
	}

private:
	SceneInstance& scene;
	std::vector<Entity> toDelete;
};