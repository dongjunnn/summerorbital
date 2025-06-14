#pragma once
#include <vector>
#include <cstdint>

struct InputState {
    bool up, down, left, right, shoot;
};

struct PlayerState {
    int id;
    float x, y;
};

struct GameState {
    std::vector<PlayerState> players;
};