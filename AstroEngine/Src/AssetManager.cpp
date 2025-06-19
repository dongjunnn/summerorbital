#include "AssetManager.h"

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int speed, std::string id)
{
	//
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