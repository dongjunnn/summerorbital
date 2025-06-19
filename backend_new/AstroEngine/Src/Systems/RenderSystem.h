#pragma once
#include "../SceneInstance.h"

class RenderSystem
{
public:
	RenderSystem(SceneInstance& sceneRef);

	void render(SDL_Renderer* renderer);

private:
	SceneInstance& scene;  
};