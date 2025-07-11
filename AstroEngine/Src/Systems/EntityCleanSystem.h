#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

class EntityCleanSystem
{
public:
	EntityCleanSystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	// cleans projectiles and returns a vector of deletions
	std::vector<Entity> cleanProjectiles()
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

		return toDelete;
	}

	std::vector<Entity> clearDeletionQueue()	// this deletes everything in sceneInstance deletion queue
	{
		toDelete.clear();

		for (Entity entity : scene.GetDeletionQueue())
		{
			toDelete.push_back(entity);		// is this step necessary? im just doing it cuz the above does it too lol
		}

		for (Entity e : toDelete)
		{
			scene.DestroyEntity(e);
			std::cout << "Destroyed Entity  " << e << std::endl;
		}

		scene.ClearDeletionQueue();		// then tells sceneInstance that everything has been deleted
		return toDelete;
	}

	void handleDefeatedPlayers()		// this could be event driven instead
	{												// like only check when a player is hit
		auto& players = scene.GetView<PlayerComponent>();	// this should do for now though

		for (Entity player : players)
		{
			int& hp = scene.GetEntityData<HealthComponent>(player).hp;
			if (hp <= 0)
			{
				PlayerComponent& plyr = scene.GetEntityData<PlayerComponent>(player);
				plyr.isAlive = false;
			}
		}
	}

private:
	SceneInstance& scene;
	std::vector<Entity> toDelete;
};