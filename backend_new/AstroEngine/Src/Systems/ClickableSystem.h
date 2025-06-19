#pragma once
#include "../Scene.h"
#include <SDL.h>

class ClickableSystem
{
public:
	ClickableSystem(SceneInstance& scene) : scene(scene) {}

	void ProcessClicks(const SDL_Event& event)
	{
		if (event.type != SDL_MOUSEBUTTONDOWN || event.button.button != SDL_BUTTON_LEFT) { return; }

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		SDL_Point mousePoint = { mouseX, mouseY };

		auto& clickable = scene.GetCompoenentUserData<ClickableComponent>();
		for (auto& e : clickable)
		{
			if (SDL_PointInRect(&mousePoint, &e.btnArea))
			{
				// debug
				std::cout << "clicked!" << std::endl;
				if (e.onClick)
				{
					e.onClick();
				}
			}
		}
	}

private:
	SceneInstance& scene;
};