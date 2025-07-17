#pragma once
#include <SDL.h>

struct SpriteComponent
{
	SDL_Texture* texture;
	SDL_Rect srcRect, dstRect;
	uint8_t alpha = 255;
};


inline std::ostream& operator<<(std::ostream& os, const SpriteComponent& sprite) {
    os << "Sprite component {\n"
        << "  texture: " << (sprite.texture ? "set" : "null") << ",\n"
        << "  srcRect: { x: " << sprite.srcRect.x
        << ", y: " << sprite.srcRect.y
        << ", w: " << sprite.srcRect.w
        << ", h: " << sprite.srcRect.h << " },\n"
        << "  dstRect: { x: " << sprite.dstRect.x
        << ", y: " << sprite.dstRect.y
        << ", w: " << sprite.dstRect.w
        << ", h: " << sprite.dstRect.h << " },\n"
        << "  alpha: " << static_cast<int>(sprite.alpha) << "\n"
        << "}";
    return os;
}

