#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	// game objects
	void CreateProjectile(Vector2D pos, Vector2D vel, int speed, std::string id);

	// texture management
	void AddTexture(std::string id, const char* path, SDL_Renderer* renderer);
	SDL_Texture* GetTexture(std::string id);

	// font management
	void AddFont(std::string id, const char* path, SDL_Renderer* renderer, uint8_t fontSize);
	TTF_Font* GetFont(std::string id);

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};