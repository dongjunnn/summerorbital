#pragma once
#include <cstdint>

enum class PacketType : uint8_t {
    PLAYER_INPUT,
    PROJECTILE_CREATED,
    PROJECTILE_DESTROYED,
    PROJECTILE_STATE,
    PLAYER_STATE
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
    float angle;
    int health;
    int colour;
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

struct PacketProjectileDestroyed {
    uint32_t entityID;
};

struct PacketMatchPhase {
    uint8_t code;
};

struct PacketMatchRestart {
    Entity id;
    uint8_t code;
};
