// #include "Client.h"
// #include <iostream>
// #include <vector>
// #include "ECS/Components.h"
// #include "Packet.h"  
// #include <cstring>  

// Client::Client() : clientHost(nullptr), serverPeer(nullptr) {}

// Client::~Client() {
//     if (clientHost != nullptr) {
//         enet_host_destroy(clientHost);
//     }
//     enet_deinitialize();
// }

// bool Client::init() {
//     if (enet_initialize() != 0) {
//         std::cerr << "An error occurred while initializing ENet." << std::endl;
//         return false;
//     }

//     clientHost = enet_host_create(NULL, 1, 2, 0, 0);
//     if (clientHost == nullptr) {
//         std::cerr << "An error occurred while trying to create a client host." << std::endl;
//         return false;
//     }

//     ENetAddress address;
//     enet_address_set_host(&address, "127.0.0.1"); // localhost
//     address.port = 1234;

//     serverPeer = enet_host_connect(clientHost, &address, 2, 0);
//     if (serverPeer == nullptr) {
//         std::cerr << "No available peers for initiating an ENet connection." << std::endl;
//         return false;
//     }

//     // Initialize our rendering engine
//     game.init("AstroEngine Client", 800, 600, false);
//     return true;
// }

// void Client::run() {
//     const int FPS = 60;
//     const int frameDelay = 1000 / FPS;
    
//     while (game.running()) {
//         Uint32 frameStart = SDL_GetTicks();

//         game.handleEvents();
//         handleInput();
//         updateFromServer();

//         game.update();
//         game.render();

//         int frameTime = SDL_GetTicks() - frameStart;
//         if (frameDelay > frameTime) {
//             SDL_Delay(frameDelay - frameTime);
//         }
//     }
// }

// // --- NEWLY IMPLEMENTED ---
// void Client::handleInput() {
//     // Create an input packet to send to the server
//     PlayerInputPacket input; // From Packet.h

//     // Get a snapshot of the current state of all keys
//     const Uint8* keystates = SDL_GetKeyboardState(NULL);

//     // Populate the packet based on which keys are currently held down
//     if (keystates[SDL_SCANCODE_W]) input.up = true;
//     if (keystates[SDL_SCANCODE_S]) input.down = true;
//     if (keystates[SDL_SCANCODE_A]) input.left = true;
//     if (keystates[SDL_SCANCODE_D]) input.right = true;

//     // Create an ENet packet to send the input state.
//     // The flag '0' (or ENET_PACKET_FLAG_UNSEQUENCED) means this is an unreliable packet.
//     // This is good for movement data that is sent every frame.
//     ENetPacket* packet = enet_packet_create(&input,
//                                             sizeof(PlayerInputPacket),
//                                             0);

//     // Send the input packet to the server on channel 0
//     enet_peer_send(serverPeer, 0, packet);
// }

// // --- NEWLY IMPLEMENTED ---
// void Client::updateFromServer() {
//     ENetEvent event;
//     // Check for network events
//     while (enet_host_service(clientHost, &event, 0) > 0) {
//         switch (event.type) {
//             case ENET_EVENT_TYPE_CONNECT:
//                 std::cout << "[CLIENT] Connection to server succeeded." << std::endl;
//                 break;
//             case ENET_EVENT_TYPE_RECEIVE:
//             {
//                 // This is the most important part of the client.
//                 // We've received a game state packet from the server.

//                 // 1. Create a vector to hold the list of player states from the packet.
//                 // We calculate the size of the vector by dividing the total packet size
//                 // by the size of a single PlayerState object.
//                 std::vector<PlayerState> playerStates(event.packet->dataLength / sizeof(PlayerState));

//                 // 2. Copy the raw data from the packet into our vector.
//                 memcpy(playerStates.data(), event.packet->data, event.packet->dataLength);

//                 // 3. Process each state update.
//                 for (const auto& serverState : playerStates) {
//                     // Check if we already have a local entity for this player
//                     if (Game::manager.hasEntity(serverState.entityID)) {
//                         // If yes, just update its position.
//                         Entity& localPlayer = Game::manager.getEntity(serverState.entityID);
//                         auto& transform = localPlayer.getComponent<TransformComponent>();
//                         transform.position.x = serverState.x;
//                         transform.position.y = serverState.y;
//                     } else {
//                         // If no, this is a new player we haven't seen before. Create a local
//                         // representation of it.
//                         std::cout << "[CLIENT] New player detected with ID " << serverState.entityID << ". Creating local entity." << std::endl;
                        
//                         // Create the entity with the SAME ID as the one on the server
//                         Entity& localPlayer = Game::manager.addEntity(serverState.entityID);
//                         localPlayer.addComponent<TransformComponent>(serverState.x, serverState.y, 32, 32, 2);
//                         localPlayer.addComponent<SpriteComponent>("player", true); // Make it visible
//                         localPlayer.addGroup(Game::groupPlayers);
//                     }
//                 }

//                 // 4. Destroy the packet when we're done with it.
//                 enet_packet_destroy(event.packet);
//                 break;
//             }
//             case ENET_EVENT_TYPE_DISCONNECT:
//                  std::cout << "[CLIENT] Disconnected from server." << std::endl;
//                  // Here you could set a flag to take the player back to a main menu
//                 break;
//         }
//     }
// }

// AstroEngine/Src/Client.cpp

#include "Client.h"
#include <iostream>
#include <vector>
#include "Components/Components.h"
#include "Packet.h"  
#include <cstring>  
#include "GameStates/ClientStates/TitleState.h"

Client::Client() : clientHost(nullptr), serverPeer(nullptr) {}

Client::~Client() {
    delete currentState;
    if (clientHost != nullptr) {
        enet_host_destroy(clientHost);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();

    enet_deinitialize();
}

bool Client::init() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return false;
    }

    clientHost = enet_host_create(NULL, 1, 2, 0, 0);
    if (clientHost == nullptr) {
        std::cerr << "An error occurred while trying to create a client host." << std::endl;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, "127.0.0.1"); // localhost
    address.port = 1234;

    serverPeer = enet_host_connect(clientHost, &address, 2, 0);
    if (serverPeer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        return false;
    }

    createWindow("Spaced Out", 800, 640, false);    // also loads assets, needs a better name
    changeState(new TitleState());      // rendering is handled inside gamestates

    return true;
}

void Client::changeState(ClientState* newState)
{
    if (currentState) {
        currentState->onExit(*this);
        delete currentState;
    }
    currentState = newState;

    currentState->onEnter(*this);
}

void Client::run() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    
    while (isRunning) {
        Uint32 frameStart = SDL_GetTicks();
        ENetEvent event;

        currentState->handleInput(*this);
        
        // Check for network events
        while (enet_host_service(clientHost, &event, 0) > 0)
        {
            currentState->handleEnetEvent(*this, event);    // updateFromServer() lives here
        }
        currentState->update(*this);     // updating client internal state
        currentState->render(*this);

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Client::createWindow(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image Init Pass: " << std::endl;
    }

    if (!renderer) {
        std::cerr << "[ERROR] Failed to create renderer: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    
    this->assets = new AssetManager();

    assets->AddTexture("titleScreen", "assets/TitleScreen.png", renderer);  // now that renderer is not static
    assets->AddTexture("playButton", "assets/PlayBtn.png", renderer);       // sorta regret that now
    assets->AddTexture("gameBackground", "assets/lolol.png", renderer);     // had to make so many changes
    assets->AddTexture("terrain", "assets/terrain.png", renderer);
    assets->AddTexture("player", "assets/player.png", renderer);
    assets->AddTexture("orb", "assets/orb.png", renderer);
    assets->AddTexture("healthBorder", "assets/HealthBar.png", renderer);
    assets->AddTexture("healthMeter", "assets/HealthMeter.png", renderer);
}
