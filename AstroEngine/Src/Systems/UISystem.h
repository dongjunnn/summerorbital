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


	}

private:
	SceneInstance& scene;
};

