#pragma once

struct HealthComponent
{
	int hp;
};

inline std::ostream& operator<<(std::ostream& os, const HealthComponent& component)
{
	os << "Health component: hp = {"
		<< component.hp << "}";

	return os;
}