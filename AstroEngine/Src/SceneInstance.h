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
		CreateComponent<LabelComponent>();
	}

	virtual ~SceneInstance() = default;
	
	// creation helpers
	Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite);
	
    Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions);

	Entity CreatePlayer(const Vector2D position, const SpriteComponent sprite);

	Entity CreatePlayer(const Vector2D position, const Vector2D colDimensions);

	// UI methods
	Entity CreateUIHealthBorder(const Vector2D position, const SpriteComponent sprite);

	Entity CreateUIHealthMeter(const Vector2D position, const SpriteComponent sprite);

	Entity CreateUITextField(const Vector2D position, const SpriteComponent sprite);

	void AddUIElement(std::string id, Entity entity);

	void RemoveUIElement(std::string id);

	bool IsUIElement(std::string id);

	Entity GetUIElement(std::string id);

	// deletion queue methods
	std::vector<Entity>& GetDeletionQueue();

	void AppendDeletionQueue(Entity entity);

	void ClearDeletionQueue();

private:
	UIElementMap uiElementMap;
	std::vector<Entity> deletionQueue;
};