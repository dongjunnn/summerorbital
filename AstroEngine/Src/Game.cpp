#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

auto& player(manager.addEntity());


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	// player =  new GameObject("AstroEngine/assets/test.png", 0, 0);
	// enemy =  new GameObject("AstroEngine/assets/test1.webp", 50, 50);
	map = new Map("AstroEngine/assets/terrain.png", 1, 32);

	// ecs implementation
	map->LoadMap("AstroEngine/assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(500, 550);
	// player.addComponent<SpriteComponent>("AstroEngine/assets/ship_idle.png", 4, 100);
	player.addComponent<SpriteComponent>("AstroEngine/assets/player.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	background = TextureManager::LoadTexture("AstroEngine/assets/lolol.png");  // background for fun; feel free to remove
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;  //  collision detection
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}
}


void Game::render()
{
	SDL_RenderClear(renderer);
	
	SDL_RenderCopy(renderer, background, NULL, NULL);  // for background

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

