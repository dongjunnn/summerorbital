#pragma once
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <cstdint>
#include <array>      // <-- add
#include <string>     // <-- add

using Entity = std::uint32_t;   // <-- std:: scope
using ComponentID = std::uint8_t; // <-- std:: scope
const size_t MAX_ENTITIES = 100000;    // arbitrary max entities
const size_t MAX_COMPONENTS = 32;      // arbitrary max components

using UIElementMap = std::unordered_map<std::string, Entity>;

const float MAX_VEL = 0.4f;
const float VEL_DECAY = 0.02f;

const size_t MAX_PLAYERS = 16;
const size_t MAX_ACTIVE_PLAYERS = 2;

using PlayerList = std::array<Entity, MAX_PLAYERS>;
using ScoreList  = std::array<int, MAX_ACTIVE_PLAYERS>;   