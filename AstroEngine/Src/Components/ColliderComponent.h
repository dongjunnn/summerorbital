#pragma once
#include <SDL.h>
#include <string>

struct ColliderComponent
{
	SDL_Rect collider;
};

inline std::ostream& operator<<(std::ostream& os, const ColliderComponent& component)
{
	os << "Collider component: collider = {"
		<< component.collider.x << ", "
		<< component.collider.y << ", "
		<< component.collider.w << ", "
		<< component.collider.h << "}";
	return os;
}