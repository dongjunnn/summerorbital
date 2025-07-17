#pragma once

struct ProjectileComponent
{
	int dmg;
};

inline std::ostream& operator<<(std::ostream& os, const ProjectileComponent& component)
{
	os << "Projectile component: dmg = {"
		<< component.dmg << "}";

	return os;
}