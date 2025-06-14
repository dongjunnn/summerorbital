// #include "Game.h"
// #include "TextureManager.h"
// #include "Map.h"
// #include "ECS/Components.h"
// #include "Vector2D.h"
// #include "Collision.h"
// #include "AssetManager.h"

// Map* map;
// Manager manager;

// SDL_Renderer* Game::renderer = nullptr;
// SDL_Event Game::event;
// AssetManager* Game::assets = new AssetManager(&manager);

// bool Game::isRunning = false;

// auto& player(manager.addEntity());


// Game::Game()
// {}

// Game::~Game()
// {}

// void Game::init(const char* title, int width, int height, bool fullscreen)
// {
// 	int flags = 0;

// 	if (fullscreen)
// 	{
// 		flags = SDL_WINDOW_FULLSCREEN;
// 	}

// 	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
// 	{
// 		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
// 		renderer = SDL_CreateRenderer(window, -1, 0);
// 		if (renderer)
// 		{
// 			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// 		}

// 		isRunning = true;
// 	}

// 	// player =  new GameObject("AstroEngine/assets/test.png", 0, 0);
// 	// enemy =  new GameObject("AstroEngine/assets/test1.webp", 50, 50);

// 	assets->AddTexture("terrain", "AstroEngine/assets/terrain.png");
// 	assets->AddTexture("player", "AstroEngine/assets/player.png");
// 	assets->AddTexture("projectile", "AstroEngine/assets/test_projectile.png");

// 	map = new Map("terrain", 1, 32);

// 	// ecs implementation
// 	map->LoadMap("AstroEngine/assets/map.map", 25, 20);

// 	player.addComponent<TransformComponent>(500, 550);
// 	// player.addComponent<SpriteComponent>("AstroEngine/assets/ship_idle.png", 4, 100);
// 	player.addComponent<SpriteComponent>("player", true);
// 	player.addComponent<KeyboardController>();
// 	player.addComponent<ColliderComponent>("player");
// 	player.addGroup(groupPlayers);

// 	assets->CreateProjectile(Vector2D(100,100), Vector2D(0.5, 0.2), 15, "projectile");

// 	background = TextureManager::LoadTexture("AstroEngine/assets/lolol.png");  // background for fun; feel free to remove
// }

// auto& tiles(manager.getGroup(Game::groupMap));
// auto& players(manager.getGroup(Game::groupPlayers));
// auto& colliders(manager.getGroup(Game::groupColliders));
// auto& projectiles(manager.getGroup(Game::groupProjectiles));

// void Game::handleEvents()
// {

// 	SDL_PollEvent(&event);

// 	switch (event.type)
// 	{
// 	case SDL_QUIT :
// 		isRunning = false;
// 		break;
// 	default:
// 		break;
// 	}
// }

// void Game::update()
// {
// 	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;  //  collision detection
// 	Vector2D playerPos = player.getComponent<TransformComponent>().position;

// 	manager.refresh();
// 	manager.update();

// 	for (auto& c : colliders)
// 	{
// 		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
// 		if (Collision::AABB(cCol, playerCol))
// 		{
// 			player.getComponent<TransformComponent>().position = playerPos;
// 		}
// 	}

// 	for (auto& p : projectiles)
// 	{
// 		if (Collision::AABB(playerCol, p->getComponent<ColliderComponent>().collider))
// 		{
// 			p->destroy();
// 		}
// 	}
// }


// void Game::render()
// {
// 	SDL_RenderClear(renderer);
	
// 	SDL_RenderCopy(renderer, background, NULL, NULL);  // for background

// 	for (auto& t : tiles)
// 	{
// 		t->draw();
// 	}
// 	for (auto& c : colliders)
// 	{
// 		c->draw();
// 	}
// 	for (auto& p : players)
// 	{
// 		p->draw();
// 	}
// 	for (auto& p : projectiles)
// 	{
// 		p->draw();
// 	}
	
// 	SDL_RenderPresent(renderer);
// }

// void Game::clean()
// {
// 	SDL_DestroyWindow(window);
// 	SDL_DestroyRenderer(renderer);
// 	SDL_Quit();
// }


#include "Game.h"
#include "AssetManager.h"
#include "Map.h"
#include "ECS/Components.h"

// Initialize static members
Manager Game::manager; // Each client has its own "mirror" of the ECS
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
AssetManager* Game::assets = new AssetManager(&Game::manager);

// map is now a local variable in Game::init, not global
Map* map;

Game::Game() : isRunning(false), window(nullptr) {}

Game::~Game() {}

// The new, simpler init function for the client
void Game::init(const char *title, int width, int height, bool fullscreen) {
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

    // The client still needs to load assets to be able to draw them
    assets->AddTexture("terrain", "assets/terrain.png");
	assets->AddTexture("player", "assets/player.png");
	assets->AddTexture("projectile", "assets/test_projectile.png");

    // The client also needs to create the map to draw it.
    map = new Map("terrain", 3, 32);
    map->LoadMap("assets/map.map", 25, 20);

    // --- LOGIC REMOVED ---
    // The client NO LONGER creates the player or projectiles.
    // It waits for the server to send information about them.
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        isRunning = false;
    }
}

// The new, simpler update function
void Game::update() {
    manager.refresh();
    // On the client, this just updates local things like sprite animations.
    // It no longer checks for collisions.
    manager.update();

    // --- LOGIC REMOVED ---
    // All collision detection code is gone from the client's update loop.
    // This logic now lives exclusively on the server.
}

void Game::render() {
    SDL_RenderClear(renderer);

    // The render function is mostly the same, as its job is just to draw
    // whatever entities are currently in its manager.
    manager.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}