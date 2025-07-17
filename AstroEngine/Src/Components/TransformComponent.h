#pragma once
#include "../Vector2D.h"


struct TransformComponent
{
	Vector2D position;
};

inline std::ostream& operator<<(std::ostream& os, const TransformComponent& component)
{
	os << "Transform component: position = {"
		<< component.position.x << ", "
		<< component.position.x << "}";

	return os;
}