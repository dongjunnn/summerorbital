// #pragma once

// #include "SDL.h"
// #include "SDL_image.h"
// #include <iostream>
// #include <vector>
// #include "AssetManager.h"

// class AssetManager;
// class ColliderComponent;

// class Game
// {
// public:
// 	Game();
// 	~Game();

// 	void init(const char* title, int width, int height, bool fullscreen);

// 	void handleEvents();
// 	void update();
// 	bool running() { return isRunning; }
// 	void render();
// 	void clean();

// 	static SDL_Renderer* renderer;
// 	static SDL_Event event;
// 	static bool isRunning;
// 	static AssetManager* assets;

// 	enum groupLabels : std::size_t
// 	{
// 		groupMap,
// 		groupPlayers,
// 		groupColliders,
// 		groupProjectiles
// 	};

// private:
// 	SDL_Window* window;
// 	SDL_Texture* background;

// };

// AstroEngine/Src/Game.h

#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "ECS/ECS.h" // We need Manager here

class AssetManager;
class ColliderComponent;

class Game
{
public:
    Game();
    ~Game();

    // The init function is now simpler. It doesn't need to know if it's a server.
    void init(const char* title, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }

    // Static members that are useful for rendering and assets.
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static AssetManager* assets;
    static Manager manager; // This is now the CLIENT'S "mirror" of the game state

    // This enum remains useful for grouping entities.
    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
        groupColliders,
        groupProjectiles
    };

private:
    bool isRunning = false; // This is now a member variable, not static.
    SDL_Window *window;
    SDL_Texture* background; // Keep the background texture here
};
