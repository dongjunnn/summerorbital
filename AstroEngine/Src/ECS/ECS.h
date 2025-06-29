#pragma once
#include <iostream>
#include <bitset>
#include <unordered_map>

using Entity = uint32_t;
using ComponentID = uint8_t;
const size_t MAX_ENTITIES = 100000;  // arbitrary max entities
const size_t MAX_COMPONENTS = 32;  // arbitrary max components

using UIElementMap = std::unordered_map<std::string, Entity>;

const float MAX_VEL = 0.4f;
const float VEL_DECAY = 0.02f;