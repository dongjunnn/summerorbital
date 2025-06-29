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
    enet_address_set_host(&address, "25.7.168.213"); // localhost
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
    assets->AddTexture("ship", "assets/shipSprite.png", renderer);
}
