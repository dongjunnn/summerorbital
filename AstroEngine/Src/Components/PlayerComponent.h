#pragma once

struct PlayerComponent
{
	bool isAlive;
	float lastFireTime = 0.0f;
};

inline std::ostream& operator<<(std::ostream& os, const PlayerComponent& component)
{
	os << "Player component: isAlive = {"
		<< component.isAlive << "} "
		<< "lastFireTime = {" << component.lastFireTime << "}";

	return os;
}