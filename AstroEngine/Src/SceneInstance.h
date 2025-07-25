#pragma once
#include "Scene.h"
#include "Events/EventManager.h"


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

	// Event integration (optional)
	void hookEventSystem(EventManager* eventMgr) { eventManager = eventMgr; }

	EventManager* events() {
		if (eventManager == nullptr) { std::cout << "[SERVER WARN] Event system not hooked onto scene" << std::endl; }
		return eventManager; }
	
	// creation helpers
	Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const SpriteComponent sprite);
	
    Entity CreateProjectile(const Vector2D position, const Vector2D velocity, const Vector2D colDimensions);

	Entity CreatePlayer(const Vector2D position, const SpriteComponent sprite);

	Entity CreatePlayer(const Vector2D position, const Vector2D colDimensions);

	// UI methods
	Entity CreateUIHealthBorder(const Vector2D position, const SpriteComponent sprite);

	Entity CreateUIHealthMeter(const Vector2D position, const SpriteComponent sprite);

	Entity CreateUITextField(const Vector2D position, const std::string text, TTF_Font* font);

	Entity CreateUIButton(const Vector2D position, const ClickableComponent click, const std::string text, TTF_Font* font, SpriteComponent sprite);

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
	EventManager* eventManager = nullptr;
};