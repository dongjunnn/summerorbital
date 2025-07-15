
#pragma once
#include <enet/enet.h>
#include "ECS/ECS.h"
#include "Map.h"
#include "GameStates/ServerStates/PlayState_S.h"
#include "Events/EventManager.h"

class Server {
public:
    Server();
    ~Server();

    bool init();
    void run();
    void changeState(ServerState* newState);

    EventManager* getEvents() { return eventManager; }
    ENetHost* getServerHost() const { return serverHost; }  // getter function for the broadcast function
                                                            // if theres a better solution go for it
private:
    ENetHost* serverHost;
    ServerState* currentState = nullptr;
    EventManager* eventManager = nullptr;

    bool isRunning = false;

    void setupGame();

};