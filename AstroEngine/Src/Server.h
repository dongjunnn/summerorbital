
#pragma once
#include <enet/enet.h>
#include "ECS/ECS.h"
#include "Game.h" // For groupLabels enum
#include "Map.h"

class Server {
public:
    Server();
    ~Server();

    bool init();
    void run();

private:
    ENetHost* serverHost;
    Manager manager;
    Map* map;
    bool isRunning = false;

    void setupGame();
    void update();
    void broadcastGameState();
};