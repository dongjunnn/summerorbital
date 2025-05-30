#pragma once
#include <string>

class Map
{
public:

    Map(std::string tID, int mscale, int tsize);
    ~Map();

    void LoadMap(std::string path, int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    std::string texID;
    int mapScale;
    int tileSize;

};