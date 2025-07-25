// AstroEngine/Src/Server.cpp

#include "Server.h"
#include <iostream>
#include "Packet.h"
#include <cstring>    
#include "Collision.h"

// Notice the server doesn't include anything related to TextureManager or rendering.

Server::Server() : serverHost(nullptr) {}

Server::~Server()
{
    if (currentState) {
        currentState->onExit(*this);
        delete currentState;
    }

    if (serverHost != nullptr) {
        enet_host_destroy(serverHost);
    }

    if (eventManager) {
        delete eventManager;
    }
    // We can call enet_deinitialize() here if the server is the only part using ENet
}

bool Server::init()
{
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return false;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY; // Listen on all available network interfaces
    address.port = 1234;          // The port clients will connect to

    // Create the server host
    serverHost = enet_host_create(&address, 32, 5, 0, 0);

    if (serverHost == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    std::cout << "[SERVER] Started successfully on port 1234." << std::endl;

    // setting up our event manager
    eventManager = new EventManager();
    if (eventManager == nullptr) {
        std::cerr << "Event Manager setup failure" << std::endl;
    }

    // Now, set up the game world
    setupGame();
    return true;
}

void Server::setupGame()        // can we consolidate this with server init?
{
    std::cout << "[SERVER] Setting up game world..." << std::endl;
    // For now, we won't create any player entities. The server will do that
    // when a client connects. Let's just log that the world is ready.

    // map lives in server playstate now, here we set the initial state of the server
    // to be running the game, we can change this to other stuff next time,
    // like a map selector if we go there
    changeState(new PlayState_S());     

    std::cout << "[SERVER] Game world created." << std::endl;
}

void Server::run()
{
    isRunning = true;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks(); 
        // --- Network Handling ---
        ENetEvent event;
        // Check for new network events (connections, packets, disconnections)
        while (enet_host_service(serverHost, &event, 0) > 0)
        {
            currentState->handleEnetEvent(*this, event);    
        }

        // --- Game Logic Update ---
        currentState->update(*this);
        currentState->broadcastStates(*this);
        // --- Timing ---
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Server::changeState(ServerState* newState)
{
    // we can change this to preserve states next time if we want
    if (currentState)
    {
        currentState->onExit(*this);
        delete currentState;
    }
    currentState = newState;

    currentState->onEnter(*this);
}

