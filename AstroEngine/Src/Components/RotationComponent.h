#pragma once

struct RotationComponent
{
	float angle; // counting clockwise
};

inline std::ostream& operator<<(std::ostream& os, const RotationComponent& component)
{
	os << "Rotation component: angle = {"
		<< component.angle << "}";

	return os;
}
