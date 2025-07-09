#include "AssetManager.h"

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int speed, std::string id)
{
	// TODO
	
}

void AssetManager::AddTexture(std::string id, const char* path, SDL_Renderer* renderer)	// now that renderer is not static
{
	SDL_Texture* tex = TextureManager::LoadTexture(path, renderer);
	if (!tex) {
		std::cerr << "Texture load failed for: " << path << std::endl;
		return; // don't emplace nullptr
	}
	textures.emplace(id, tex);
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	auto it = textures.find(id);
	if (it != textures.end()) {
		return it->second;
	}
	else {
		std::cerr << "Texture not found: " << id << std::endl;
		return nullptr;
	}
	return textures[id];
}

void AssetManager::AddFont(std::string id, const char* path, SDL_Renderer* renderer, uint8_t fontSize)
{
	TTF_Font* font = TTF_OpenFont(path, fontSize);
	if (!font) {
		std::cerr << "Font load for: " << path << std::endl;
		return;
	}
	fonts.emplace(id, font);
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	auto it = fonts.find(id);
	if (it != fonts.end()) {
		return it->second;
	}
	else {
		std::cerr << "Font not found: " << id << std::endl;
		return nullptr;
	}
	return fonts[id];
}