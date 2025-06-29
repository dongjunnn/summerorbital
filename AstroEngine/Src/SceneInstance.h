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
		CreateComponent<AnimationComponent>();
		CreateComponent<HealthComponent>();
		CreateComponent<RotationComponent>();
	}

	virtual ~SceneInstance() = default;
	
	Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite);
	
    Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions);

	Entity CreatePlayer(const Vector2D position, const SpriteComponent sprite);

	Entity CreatePlayer(const Vector2D position, const Vector2D colDimensions);

	Entity CreateUIHealthBorder(const Vector2D position, const SpriteComponent sprite);

	Entity CreateUIHealthMeter(const Vector2D position, const SpriteComponent sprite);

	void AddUIElement(std::string id, Entity entity);

	Entity GetUIElement(std::string id);

private:
	UIElementMap uiElementMap;
};