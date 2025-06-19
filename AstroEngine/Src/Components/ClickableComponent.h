#pragma once
#include <SDL.h>
#include <functional>

struct ClickableComponent
{
	SDL_Rect btnArea;
	std::function<void()> onClick;
};

