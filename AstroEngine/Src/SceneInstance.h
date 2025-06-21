#pragma once
#include "Scene.h"


class SceneInstance : public Scene
{
public:
	SceneInstance()		//  register components here
	{
		CreateComponent<TransformComponent>();
		CreateComponent<VelocityComponent>();
		CreateComponent<SpriteComponent>();
		CreateComponent<ColliderComponent>();
		CreateComponent<ClickableComponent>();
		CreateComponent<PlayerComponent>();
		CreateComponent<TileComponent>();
		CreateComponent<ProjectileComponent>();
	}

	virtual ~SceneInstance() = default;
	
	Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite);
	
    Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions);

	Entity CreatePlayer(const Vector2D position, const SpriteComponent sprite);

	Entity CreatePlayer(const Vector2D position, const Vector2D colDimensions);
};