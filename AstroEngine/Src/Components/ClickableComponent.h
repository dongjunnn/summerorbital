#pragma once
#include <SDL.h>
#include <functional>

struct ClickableComponent
{
	SDL_Rect btnArea;
	std::function<void()> onClick;
	bool isActive = true;
};

inline std::ostream& operator<<(std::ostream& os, const ClickableComponent& component)
{
	os << "Clickable component: btnArea = {"
		<< component.btnArea.x << ", "
		<< component.btnArea.y << ", "
		<< component.btnArea.w << ", "
		<< component.btnArea.h << "} "
		<< "isActive = {"
		<< component.isActive << "}";

	return os;
}