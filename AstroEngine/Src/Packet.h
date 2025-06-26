#pragma once
#include <cstdint>

enum class PacketType : uint8_t {
    PLAYER_INPUT,
    PROJECTILE_CREATED
};



struct PlayerInputPacket {
    //PacketType type = PacketType::PLAYER_INPUT;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool fireButtonPressed = false;
};


struct PlayerState {
    uint32_t entityID; // The unique ID of the entity
    float x;
    float y;
};

struct ProjectileState {
    Entity projectileID; // The unique ID of the projectile
    float x;
    float y;
};

struct PacketProjectileCreated {
    //PacketType type = PacketType::PROJECTILE_CREATED;
    uint32_t entityID;
    Vector2D position;
    Vector2D velocity;
};
