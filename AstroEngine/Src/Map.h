#pragma once
#include <string>
#include "SceneInstance.h"
#include "AssetManager.h"

class Map
{
public:

    Map(std::string tID, int mscale, int tsize);
    ~Map();

    void LoadMap(std::string path, SceneInstance& scene, int sizeX, int sizeY);
    void LoadMap(std::string path, SceneInstance& scene, AssetManager& assets, int sizeX, int sizeY); // hope the dereferencing doesnt bite
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    std::string texID;
    int mapScale;
    int tileSize;

};