#include "Map.h"
#include "Game.h"
#include "ECS/ECS.h"
#include "ECS/Components.h"
#include <fstream>

extern Manager manager;

Map::Map(std::string tID, int mscale, int tsize) : texID(tID), mapScale(mscale), tileSize(tsize)
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c);
			mapFile.get(c);
			srcX = atoi(&c);

			int index = srcY * 10 + srcX;
			srcY = (index / 8) * tileSize;
			srcX = (index % 8) * tileSize;

			AddTile(srcX, srcY, x * (tileSize * mapScale), y * (tileSize * mapScale));

			if (index != 24)
			{
				auto& terrainCol(manager.addEntity());
				terrainCol.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale);
				terrainCol.addGroup(Game::groupColliders);
			}

			mapFile.ignore();

		}
	}
	
	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}


Map::~Map()
{
}
   

