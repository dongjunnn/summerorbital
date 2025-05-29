#pragma once
#include <string>

class Map
{
public:

    Map(const char* path, int mscale, int tsize);
    ~Map();

    void LoadMap(std::string path, int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    const char* mapFilePath;
    int mapScale;
    int tileSize;

};