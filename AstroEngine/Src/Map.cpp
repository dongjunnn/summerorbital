#include "Map.h"
#include "Client.h"
#include "ECS/ECS.h"
#include "Components/Components.h"
#include <fstream>


Map::Map(std::string tID, int mscale, int tsize) : texID(tID), mapScale(mscale), tileSize(tsize)
{ }

void Map::LoadMap(std::string path, SceneInstance& scene, int sizeX, int sizeY)
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
			srcY = c - '0'; 
			mapFile.get(c);
			srcX = c - '0'; 

			int index = srcY * 10 + srcX;
			srcY = (index / 8) * tileSize;
			srcX = (index % 8) * tileSize;

			AddTile(srcX, srcY, x * (tileSize * mapScale), y * (tileSize * mapScale));

			if (index != 24)
			{
				//auto& terrainCol(manager.addEntity());
				//terrainCol.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale);
				//terrainCol.addGroup(Game::groupColliders);

				Entity entity = scene.CreateEntity();
				scene.AddComponent<TransformComponent>(entity);
				scene.AddComponent<ColliderComponent>(entity);
				scene.AddComponent<TileComponent>(entity);	// tilecomponent has no data, just a flag
				
				TransformComponent tfm = { Vector2D(x * (tileSize * mapScale), y * (tileSize * mapScale)) };
				ColliderComponent cdr = { { x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale, tileSize * mapScale } };

				scene.SetEntityData<TransformComponent>(entity, tfm);
				scene.SetEntityData<ColliderComponent>(entity, cdr);
			}

			mapFile.ignore();

		}
	}
	
	mapFile.close();
}

void Map::LoadMap(std::string path, SceneInstance& scene, AssetManager& assets, int sizeX, int sizeY)
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
			srcY = c - '0'; 
			mapFile.get(c);
			srcX = c - '0';

			int index = srcY * 10 + srcX;
			srcY = (index / 8) * tileSize;
			srcX = (index % 8) * tileSize;

			AddTile(srcX, srcY, x * (tileSize * mapScale), y * (tileSize * mapScale));

			if (index != 24)
			{
				//auto& terrainCol(manager.addEntity());
				//terrainCol.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale);
				//terrainCol.addGroup(Game::groupColliders);

				Entity entity = scene.CreateEntity();
				scene.AddComponent<TransformComponent>(entity);
				scene.AddComponent<SpriteComponent>(entity);
				scene.AddComponent<ColliderComponent>(entity);
				scene.AddComponent<TileComponent>(entity);	// tilecomponent has no data, just a flag

				TransformComponent tfm = { Vector2D(x * (tileSize * mapScale), y * (tileSize * mapScale)) };
				SpriteComponent spr = {
					assets.GetTexture("terrain"),
					{srcX, srcY, tileSize, tileSize},
					{x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale, tileSize * mapScale}
				};
				ColliderComponent cdr = { { x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale, tileSize * mapScale } };

				scene.SetEntityData<TransformComponent>(entity, tfm);
				scene.SetEntityData<SpriteComponent>(entity, spr);
				scene.SetEntityData<ColliderComponent>(entity, cdr);
			}

			mapFile.ignore();

		}
	}

	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	//auto& tile(manager.addEntity());
	//tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	//tile.addGroup(Game::groupMap);
}


Map::~Map()
{
}
   

