#pragma once
#include <unordered_map>
#include <string>

struct AnimationData
{
    int index;
    int frames;
    int speed;
};

struct AnimationComponent
{
    std::unordered_map<std::string, AnimationData> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
};


