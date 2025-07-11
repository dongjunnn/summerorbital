#include "ClientData.h"

void ClientData::assignPlayer(Entity id)
{
    if (!p1)
    {
        p1 = id;
        std::cout << "[SERVER] Entity " << id << " is Player #1" << std::endl;
    }
    else if (!p2)
    {
        p2 = id;
        std::cout << "[SERVER] Entity " << id << " is Player #2" << std::endl;
    }
    else
    {
        std::cout << "[SERVER] Room is full, third player currently not supported, expect crashes lol " << std::endl;
    }
}

void ClientData::disassignPlayer(Entity id)
{
    if (p1 == id)
    {
        p1 = 0;
        std::cout << "[SERVER] Player #1 position freed" << std::endl;
    }
    else if (p2 == id)
    {
        p2 = 0;
        std::cout << "[SERVER] Player #2 position freed" << std::endl;
    }
}

int ClientData::getPlayerRefbyID(Entity id) const
{
    if (p1 == id)
    {
        return 1;
    }
    else if (p2 == id)
    {
        return 2;
    }
    else
    {
        return 0;   // if bad id or third player etc
    }
}

Entity ClientData::getPlayerIDbyRef(int reference)
{
    if (reference == 1) { return p1; } else
    if (reference == 2) { return p2; }
    else { return 0; std::cerr << "bad player reference: " << reference << std::endl; }
}

