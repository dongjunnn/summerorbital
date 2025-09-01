#pragma once
#include <enet/enet.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "AssetManager.h"
#include "GameStates/ClientStates/ClientState.h"
#include "Events/EventManager.h"

class AssetManager;

class Client {
public:
    Client();
    ~Client();

    bool init();
    void run();
    void changeState(ClientState* newState);

    SDL_Renderer* renderer;
    SDL_Event event;
    AssetManager* assets;   // assets belongs to the client now

    EventManager* getEvents() { return eventManager; }
    ENetPeer* getServerPeer() const { return serverPeer; }      // again if theres a better way
    void setServerPeer(ENetPeer* peer) { serverPeer = peer; }

    ENetHost* getClientHost() const { return clientHost; }
    bool isRunning = false; // if theres a better way to do this go ahead

private:
    ENetHost* clientHost;
    ENetPeer* serverPeer;
    EventManager* eventManager = nullptr;   // one day maybe static events or smth

    SDL_Window* window;

    ClientState* currentState = nullptr;

    // can change this if you want
    void createWindow(const char* title, int width, int height, bool fullscreen);

};