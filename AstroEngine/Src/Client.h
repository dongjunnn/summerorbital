#pragma once
#include <enet/enet.h>
#include "Game.h"

class Client {
public:
    Client();
    ~Client();

    bool init();
    void run();

private:
    ENetHost* clientHost;
    ENetPeer* serverPeer;
    Game game; // The Client owns the Game engine
    bool isRunning = false;

    void handleInput();
    void updateFromServer();
};