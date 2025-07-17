#pragma once

struct VelocityComponent
{
	Vector2D velocity;
};

inline std::ostream& operator<<(std::ostream& os, const VelocityComponent& component)
{
	os << "Velocity component: position = {"
		<< component.velocity.x << ", "
		<< component.velocity.y << "}";

	return os;
}