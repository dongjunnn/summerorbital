#pragma once

struct TileComponent
{
	// doesnt contain anything, just a flag
};

inline std::ostream& operator<<(std::ostream& os, const TileComponent& component)
{
	os << "Tile component: has tile component";

	return os;
}
