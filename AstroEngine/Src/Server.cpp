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
    serverHost = enet_host_create(&address, 32, 2, 0, 0);

    if (serverHost == nullptr) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return false;
    }

    std::cout << "[SERVER] Started successfully on port 1234." << std::endl;

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
        Uint32 frameStart = SDL_GetTicks(); // We can use SDL timing functions

        // --- Network Handling ---
        ENetEvent event;
        // Check for new network events (connections, packets, disconnections)
        while (enet_host_service(serverHost, &event, 0) > 0)
        {
            currentState->handleEnetEvent(*this, event);    // the logic is now up to server states to decide
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

/* now lives in systems
void Server::update()
{
    // These two lines run the component updates (e.g., moving entities based on velocity)
    manager.refresh();
    manager.update();

    // --- COLLISION DETECTION LOGIC ---
    // This logic is moved from your old Game.cpp and now runs on the server.

    auto& colliders = manager.getGroup(Game::groupColliders);
    auto& players = manager.getGroup(Game::groupPlayers);
    auto& projectiles = manager.getGroup(Game::groupProjectiles);

    // We loop through each player to check their collisions individually
    for (auto& player : players)
    {
        SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
        Vector2D playerPos = player->getComponent<TransformComponent>().position; // Store pre-collision position

        // 1. Check for player vs. map tile collisions
        for (auto& c : colliders)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(cCol, playerCol))
            {
                // On collision, reset the player's position to where they were just before moving.
                // This is a simple way to stop them from walking through walls.
                player->getComponent<TransformComponent>().position = playerPos;
            }
        }

        // 2. Check for player vs. projectile collisions
        for (auto& p : projectiles)
        {
            // Make sure the projectile doesn't hit the player who shot it (if needed)
            // You might need a "ownerID" in your projectile component for this.

            SDL_Rect projectileCol = p->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(playerCol, projectileCol))
            {
                std::cout << "[SERVER] Player " << player->getID() << " was hit by a projectile!" << std::endl;
                p->destroy(); // Destroy the projectile
                
                // Here you would also apply damage to the player, for example:
                // player->getComponent<StatsComponent>().health -= 10;
            }
        }
    }
    
    // After all logic is done, send the final, corrected positions to all clients.
    broadcastGameState();
}
*/

/*
void Server::broadcastGameState()
{
    // This function will eventually package up the game state (like entity positions)
    // and send it to all clients. For now, we can leave it empty.
    std::vector<PlayerState> playerStates;
    for (auto& player : manager.getGroup(Game::groupPlayers))
    {
        PlayerState state;
        state.entityID = player->getID();
        state.x = player->getComponent<TransformComponent>().position.x;
        state.y = player->getComponent<TransformComponent>().position.y;
        playerStates.push_back(state);
    }

    // Only send a packet if there are players to update
    if (!playerStates.empty())
    {
        // Create an ENet packet.
        // The data is the start of our vector, and the length is the total size in bytes.
        // ENET_PACKET_FLAG_RELIABLE means the packet is guaranteed to arrive.
        ENetPacket* packet = enet_packet_create(playerStates.data(),
                                                playerStates.size() * sizeof(PlayerState),
                                                ENET_PACKET_FLAG_RELIABLE);

        // Send the packet to all connected clients on channel 0.
        enet_host_broadcast(serverHost, 0, packet);
    }
}
*/