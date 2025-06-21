#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"
#include "../Collision.h"


enum class CollisionType
{
	PlayerTile,
	PlayerProjectile
};

struct CollisionEvent
{
	Entity a;
	Entity b;
	CollisionType type;
};

class CollisionSystem
{
public:
	CollisionSystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	void updateColliderPositions()	// updates all collidercomponent positions
	{
		auto& view = scene.GetView<ColliderComponent, TransformComponent>();

		for (Entity e : view)
		{
			TransformComponent& t = scene.GetEntityData<TransformComponent>(e);
			ColliderComponent& c = scene.GetEntityData<ColliderComponent>(e);

			c.collider.x = t.position.x;
			c.collider.y = t.position.y;
		}
	}

	void checkCollision()
	{
		CollisionEvents.clear();	// clears the previous tick's events

		// checks for player-tile collisions, then player-projectile collisions
		// this style is not very extensible, but hopefully it wont have to 
		auto& tiles = scene.GetView<TileComponent>();
		auto& players = scene.GetView<PlayerComponent>();
		auto& projs = scene.GetView<ProjectileComponent>();

		for (Entity player : players)
		{	
			// player-tile
			SDL_Rect& pcol = scene.GetEntityData<ColliderComponent>(player).collider;
			for (Entity tile : tiles)
			{
				SDL_Rect& tcol = scene.GetEntityData<ColliderComponent>(tile).collider;
				if (Collision::AABB(pcol, tcol))
				{
					CollisionEvents.push_back({ player, tile, CollisionType::PlayerTile });
				}
			}
			// player-projectile
			for (Entity proj : projs)
			{
				SDL_Rect& projcol = scene.GetEntityData<ColliderComponent>(proj).collider;
				if (Collision::AABB(pcol, projcol))
				{
					CollisionEvents.push_back({ player, proj, CollisionType::PlayerProjectile });
				}
			}
		}
	}

	void resolveCollision()
	{
		for (const auto& colEvent : CollisionEvents)
		{
			switch (colEvent.type)
			{
			case CollisionType::PlayerTile:		// player tile events are (player, tile, event)
			{
				std::cout << "playertile resolution part" << std::endl;

				SDL_Rect& pcol = scene.GetEntityData<ColliderComponent>(colEvent.a).collider;
				SDL_Rect& tcol = scene.GetEntityData<ColliderComponent>(colEvent.b).collider;

				TransformComponent& ptfm = scene.GetEntityData<TransformComponent>(colEvent.a);

				// finding the distance between centers
				int dx = (pcol.x + pcol.w / 2) - (tcol.x + tcol.w / 2);
				int dy = (pcol.y + pcol.h / 2) - (tcol.y + tcol.h / 2);

				// checking if their distance is smaller than the sum of half their widths per axis
				int overlapX = (pcol.w + tcol.w) / 2 - std::abs(dx);
				int overlapY = (pcol.h + tcol.h) / 2 - std::abs(dy);

				if (overlapX < overlapY)
				{
					int correction = (dx > 0) ? overlapX : -overlapX;		// hehe funny c++ syntax
					pcol.x += correction;									// move the player by the overlap amount
					ptfm.position.x += correction;
				}
				else
				{
					int correction = (dy > 0) ? overlapY : -overlapY;
					pcol.y += correction;
					ptfm.position.y += correction;
				}
				break;
			}
			case CollisionType::PlayerProjectile:
			{
				std::cout << "player-projectile resolution part" << std::endl;	// player projectile events are (player, projectile, event)
				
				// for now we just destroy projectile
				// add health logic later
				
				scene.DestroyEntity(colEvent.b);		
				std::cout << "Projectile destroyed?" << scene.isActive(colEvent.b) << std::endl;
				break;
			}
			default:
				break;
			}
		}
	}

private:
	SceneInstance& scene;
	std::vector<CollisionEvent> CollisionEvents;
};
