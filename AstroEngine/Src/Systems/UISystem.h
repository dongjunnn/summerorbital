#pragma once
#include "../SceneInstance.h"
#include "../Components/Components.h"

class UISystem	
{
public:
	UISystem(SceneInstance& sceneRef) : scene(sceneRef) {}

	void displayHP()
	{
		Entity healthBorder = scene.GetUIElement("HealthBorder");
		Entity healthBar = scene.GetUIElement("HealthBar");

		int& healthBarLevelsrc = scene.GetEntityData<SpriteComponent>(healthBar).srcRect.w;	// max hp hbl = 224 min hp hbl = 64
		int& healthBarLeveldst = scene.GetEntityData<SpriteComponent>(healthBar).dstRect.w;

		Entity thisPlayer = scene.GetUIElement("thisPlayer");
		int& thisHP = scene.GetEntityData<HealthComponent>(thisPlayer).hp;

		if (scene.GetEntityData<HealthComponent>(thisPlayer).hp <= 0)
		{
			bool& thisAlive = scene.GetEntityData<PlayerComponent>(thisPlayer).isAlive;
			thisAlive = false;
			healthBarLevelsrc = healthBarLeveldst = 64;
			return;
		}

		healthBarLevelsrc = healthBarLeveldst = (int)(64.0f + (thisHP / 100.0f) * 160.0f);     // currently players all have 100hp
	}

private:
	SceneInstance& scene;
};

