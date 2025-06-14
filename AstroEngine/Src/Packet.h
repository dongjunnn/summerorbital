#pragma once
#include <cstdint>

struct PlayerInputPacket {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};


struct PlayerState {
    uint32_t entityID; // The unique ID of the entity
    float x;
    float y;
};